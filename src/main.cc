#include <iostream>
#include <string>
#include <signal.h>

#include "server.hh"
#include "info.hh"

using std::cout, std::cerr;
using std::endl;
using std::string;

volatile int end = 0;

void interruptHandler(int) {
    cerr << endl << "Exiting..." << endl;
    //end = 1;
    exit(EXIT_SUCCESS);
}


string secretHandler(string) {
    return "bruh";
}


int main(int argc, char const *argv[]) {
    unsigned port;

    signal(SIGINT, interruptHandler);

    // Read the arguments
    bool argValid = false;

    if (argc == 2) {
        port = stoi(string(argv[1]));
        argValid = true;
    }

    if (!argValid) {
        cerr << "Usage: " << argv[0] << " PORT" << endl;
        exit(EXIT_FAILURE);
    }

    // Set the server up
    Server srv(port);
    srv.entries = {
        {"/", getIndex},
        {"/hostname", getHostname},
        {"/cpu-name", getCpuInfo},
        {"/load", getCpuUsage},
    };

    // Start the server
    srv.init();

    while (!end) {
        srv.handleConnection();
    }

    srv.end();

    return EXIT_SUCCESS;
}
