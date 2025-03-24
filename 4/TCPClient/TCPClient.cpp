#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>


class TCPClient {
private:
    WSADATA _wsa_;
    SOCKET _socket_;
    sockaddr_in _server_addr_{}, _client_addr_{};
    char* _rec_buff_ = new char[32];
    int _port_ = 12345;
public:
    TCPClient() {
        int err = WSAStartup(MAKEWORD(2, 2), &_wsa_);
        //_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
        _socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        _server_addr_.sin_family = AF_INET;
        std::string _ip_{ "127.0.0.1" };
        _server_addr_.sin_addr.s_addr = inet_addr(_ip_.c_str());
        _server_addr_.sin_port = htons(_port_);
    }
    ~TCPClient() {
        closesocket(_socket_);
        WSACleanup();
    }

    void send() {
        int server_size = sizeof(_server_addr_);
        std::string msg{ "Hello from Client." };
        sendto(_socket_, msg.c_str(), msg.size(), 0, (const struct sockaddr*)&_server_addr_, server_size);
        int size = recvfrom(_socket_, _rec_buff_, 32, 0, (sockaddr*)&_server_addr_, &server_size);
        if (size > 0) {
            std::cout << _rec_buff_ << std::endl;
        }
    }
};

int main()
{
    TCPClient test;
    test.send();
    return 0;
}