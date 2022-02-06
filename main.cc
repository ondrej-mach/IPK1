#include <iostream>
#include <string>
#include <signal.h>

#include "server.hh"
#include "info.hh"

using std::cout, std::cerr;
using std::endl;
using std::string;


void interruptHandler(int) {
    cerr << endl << "Exiting..." << endl;
    exit(EXIT_SUCCESS);
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

    // Infinite loop, exited only with interrupt
    while (true) {
        srv.handleConnection();
    }

    srv.end();

    return EXIT_SUCCESS;
}
