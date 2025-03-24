
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>


class TCPServer {
private:
    WSADATA _wsa_;
    SOCKET _socket_;
    sockaddr_in _server_addr_{}, _client_addr_{};
    char* _rec_buff_ = new char[32];
    int _port_ = 12345;
public:
    TCPServer() {
        int err = WSAStartup(MAKEWORD(2, 2), &_wsa_);
        //_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
        _socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        _server_addr_.sin_family = AF_INET;
        _server_addr_.sin_addr.s_addr = INADDR_ANY;
        _server_addr_.sin_port = htons(_port_);
        err = bind(_socket_, (sockaddr*)&_server_addr_, sizeof(_server_addr_));
        if (err == SOCKET_ERROR) {
            std::cout << "SOCKET NOT CREATED" << std::endl;
            closesocket(_socket_);
            WSACleanup();
        }
    }
    ~TCPServer() {
        closesocket(_socket_);
        WSACleanup();
    }

    void resieve_and_answer() {
        int client_size = sizeof(_client_addr_);
        while (true) {
            std::cout << "Wait for client..." << std::endl;
            int size = recvfrom(_socket_, _rec_buff_, 32, 0, (sockaddr*)&_client_addr_, &client_size);
            if (size > 0) {
                std::cout << _rec_buff_ << std::endl;
                std::string answer{ "Hello from Server." };
                sendto(_socket_, answer.c_str(), answer.size(), 0, (const struct sockaddr*)&_client_addr_, client_size);
            }
        }
    }
};

int main()
{
    TCPServer test;
    test.resieve_and_answer();
    return 0;
}

