#include "info.hh"

#include <fstream>
#include <string>

#include <cstring>
#include <unistd.h>


using std::string;
using std::ifstream;
using std::endl;

string getIndex(string) {
    return "Hello there!\ntry /hostname, /cpu-name or /load to display system details.";
}


string getHostname(string) {
    const int MAX_LEN = 128;
    char name[MAX_LEN] = {0};

    // Call unix function
    gethostname(name, MAX_LEN);

    return name;
}


string getCpuInfo(string) {
    ifstream cpuinfo;
    cpuinfo.open("/proc/cpuinfo");

    const string target = "model name";
    string model;
    string line;

    while (std::getline(cpuinfo, line)) {
        if (line.substr(0, target.length()) == target) {
            model = line.substr(line.find(":")+2, line.length());
        }
    }

    return model;
}

float readCpuUsage() {
    static unsigned long idleOld=0, busyOld=0;
    unsigned long user, nice, system, idle, busy;
    string cpuString;
    ifstream stat;

    stat.open("/proc/stat");
    stat >> cpuString >> user >> nice >> system >> idle;
    busy = user + nice + system;

    unsigned long busyDelta = busy - busyOld;
    unsigned long idleDelta = idle - idleOld;

    busyOld = busy;
    idleOld = idle;

    float usage;

    if (busyDelta == 0) {
        usage = 0;
    } else {
        usage = float(busyDelta) / float(busyDelta+idleDelta);
    }

    return usage;
}

string getCpuUsage(string) {
    readCpuUsage();

    // Wait for 0.2s
    usleep(200000);
    // Then take the next measurement
    float reading = readCpuUsage();

    return std::to_string(int(100.0 * reading)) + "%";
}
