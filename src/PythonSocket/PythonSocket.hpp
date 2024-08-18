// Socket.h
#ifndef PYTHON_SOCKET_HPP
#define PYTHON_SOCKET_HPP

#include <string>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>

class PythonSocket
{
public:
    PythonSocket(const std::string &host, int port);
    ~PythonSocket();

    void connect();
    void send(const std::string &data);
    std::string receive();
    void close();

private:
    std::string host_;
    int port_;
    int sock_;
    struct sockaddr_in server_addr_;
};

#endif // SOCKET_HPP