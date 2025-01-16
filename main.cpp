#include <iostream>
#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Socket
{
private:
    int _sockfd;
    struct sockaddr_in _servaddr;

public:
    Socket(int port) : _sockfd(socket(AF_INET, SOCK_STREAM, 0))
    {
        if (_sockfd == -1)
        {
            throw std::runtime_error("Socket creation failed");
        }
        memset(&_servaddr, 0, sizeof(_servaddr));
        _servaddr.sin_family = AF_INET;
        _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        _servaddr.sin_port = htons(port);
    }

    ~Socket()
    {
        if (_sockfd != -1)
        {
            close(_sockfd);
        }
    }

    void bindAndListen()
    {
        if (bind(_sockfd, (struct sockaddr *)&_servaddr, sizeof(_servaddr)) < 0)
        {
            throw std::runtime_error("Socket listen failed");
        }
    }

    int acceptClient(struct sockaddr_in &clientAddr)
    {
        socklen_t clientLen = sizeof(clientAddr);
        int clientSockFd = accept(_sockfd, (struct sockaddr *)&clientAddr, &clientLen);
        if (clientSockFd < 0)
        {
            throw std::runtime_error("Failed to accept connection");
        }
        return clientSockFd;
    }

    std::string pullMessage()
    {
        return ("Totally not pulled message");
    }
};

class Server
{
private:
    Socket _listeningSocket;

public:
    Server(int port) : _listeningSocket(port) {}

    int run()
    {
        try
        {
            _listeningSocket.bindAndListen();
            // Ready to accept connections. Logic for acception connection would go here.
            return 0;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error during server run: " << e.what() << std::endl;
            return 1; // Return an error code if server fail to start
        }
    }
};

int main()
{
    Server server(8081);
    return server.run();
}