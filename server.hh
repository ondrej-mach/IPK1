#include <string>
#include <vector>
#include <functional>

struct Entry {
    std::string pathname;
    std::function<std::string(std::string)> handler;
};

class Server {
    private:
        unsigned port;
        int socket_fd;
        std::string getResponse(std::string req);

    public:
        std::vector<Entry> entries;

        Server(unsigned port);
        int init();
        int end();
        int handleConnection();
};
