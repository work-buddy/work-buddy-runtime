#include "PythonSocket.hpp"

PythonSocket::PythonSocket(const std::string &host, int port)
    : host_(host), port_(port), sock_(-1)
{
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0)
    {
        throw std::runtime_error("Socket creation error");
    }

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);

    if (inet_pton(AF_INET, host_.c_str(), &server_addr_.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid address/ Address not supported");
    }
}

PythonSocket::~PythonSocket()
{
    close();
}

void PythonSocket::connect()
{
    if (::connect(sock_, (struct sockaddr *)&server_addr_, sizeof(server_addr_)) < 0)
    {
        throw std::runtime_error("Connection Failed");
    }
}

void PythonSocket::send(const std::string &data)
{
    if (::send(sock_, data.c_str(), data.length(), 0) < 0)
    {
        throw std::runtime_error("Send failed");
    }
}

std::string PythonSocket::receive()
{
    char buffer[1024] = {0};
    int valread = read(sock_, buffer, 1024);
    if (valread < 0)
    {
        throw std::runtime_error("Receive failed");
    }
    return std::string(buffer, valread);
}

void PythonSocket::close()
{
    if (sock_ >= 0)
    {
        ::close(sock_);
        sock_ = -1;
    }
}