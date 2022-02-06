#include "server.hh"

#include <iostream>
#include <sstream>

#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using std::string, std::vector, std::tuple;
using std::cerr;
using std::endl;
using std::function;
using std::max, std::min;

Server::Server(unsigned port) {
    this->port = port;
}

int Server::init() {
    const int backlog = 10;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == 0) {
        cerr << "Error in socket call: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Specify address before binding
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        cerr << "Error in bind call: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, backlog) < 0) {
        cerr << "Error in listen call: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}

int Server::end() {
    close(socket_fd);

    return 0;
}

int Server::handleConnection() {

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    cerr << "Waiting for new connection...\n";

    int client_fd = accept(socket_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);

    if (client_fd < 0) {
        cerr << "Error in accept call: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    char buffer[30000] = {0};
    //long nRead;
    //nRead = read(client_fd , buffer, 30000);
    read(client_fd , buffer, 30000);

    string request = buffer;
    string response = getResponse(request);

    write(client_fd, response.c_str(), response.length());
    cerr << "Response sent." << endl;

    close(client_fd);
    cerr << "Client disconnected." << endl;

    return 0;
}

string Server::getResponse(string req) {
    cerr << "Processing request:" << endl << req << endl;

    // Parse all the stuff
    string line = req.substr(0, req.find("\n"));

    int spacePos = line.find(" ");
    string method = line.substr(0, spacePos);
    line = line.substr(spacePos+1, line.length());

    spacePos = line.find(" ");
    string url = line.substr(0, spacePos);
    string httpVer = line.substr(spacePos+1, line.length());

    int pathnameBegin = url.rfind('/');
    int pathnameEnd = url.length();
    string pathname;

    if (pathnameBegin == -1) {
        pathname = "/";
    } else {
        pathname = url.substr(pathnameBegin, pathnameEnd);
    }

    // Craft the response
    std::ostringstream r;
    r << "HTTP/1.1 ";
    string responseBody;

    bool valid = true;

    if (valid && (method != "GET")) {
        r << "405 Method Not Allowed\n";
        valid = false;
    }

    if (valid && (httpVer.substr(0, 4) != "HTTP")) {
        cerr << httpVer.substr(0, 4);
        r << "400 Bad Request\n";
    }

    if (valid) {
        bool found = false;
        for (auto entry: entries) {
            if (entry.pathname == pathname) {
                r << "200 OK\n";
                responseBody = entry.handler(req);
                found = true;
                break;
            }
        }

        if (!found) {
            r << "404 Not Found\n";
            valid = false;
        }
    }

    if (!valid && (responseBody == "")) {
        responseBody = "Invalid request";
    }

    // Append response body
    r << "Content-Type: text/plain\n";
    r << "Content-Length: ";
    r << responseBody.length();
    r << "\n\n";
    r << responseBody;

    string response = r.str();

    cerr << "Sending response:" << endl << response << endl;
    return response;
}
