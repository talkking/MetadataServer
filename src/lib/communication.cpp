/*
 * @Author: Jie
 * @LastEditors: Jie
 * @FilePath: /src/lib/communication.cpp
 */

#include "communication.h"

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <iostream>


int fd_is_valid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void *GetAddress(struct sockaddr *socket_address) {
    if (socket_address->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)socket_address)->sin_addr);
    }
    return &(((struct sockaddr_in6*)socket_address)->sin6_addr);
}

std::string GetIpString(int ai_family, struct sockaddr *socket_address) {
    char ip_string[INET6_ADDRSTRLEN];
    inet_ntop(ai_family, GetAddress(socket_address), ip_string, sizeof ip_string);
    return std::string(ip_string);
}

ServerSocket::ServerSocket(int port) {
    signal(SIGPIPE, SIG_IGN);
    client_sockfd_ = -1;
    bool flag = false;
    for (int i = 0; i < 8; ++i) {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ < 0)
            continue;

        bzero((char *) &(server_address_), sizeof(server_address_));
        port_ = port;
        server_address_.sin_family = AF_INET;
        server_address_.sin_addr.s_addr = INADDR_ANY;
        server_address_.sin_port = htons(port);
        if (bind(sockfd_, (struct sockaddr *) &(server_address_), sizeof(server_address_)) < 0)
            continue;
        listen(sockfd_, 16);
        client_address_length_ = sizeof(client_address_);
        p_client_address_ = (struct sockaddr *) &(client_address_);
        flag = true;
        break;
    }
    if (!flag) {
        std::cout << "[ERROR] Cannot open socket on port " << port << "!" << std::endl;
    }
    else {
        std::cout << "[Socket] Listening on port " << port << "..." << std::endl;
    }
}

int ServerSocket::AcceptConnection() {
    client_sockfd_ = accept(sockfd_, p_client_address_, &(client_address_length_));
    client_address_string_ = GetIpString(AF_INET, p_client_address_);
    if (client_sockfd_ < 0) {
        Error("[Socket] Accept connection failed!");
    }
    return client_sockfd_;
}

std::string ServerSocket::get_client_address_string_() {
    return client_address_string_;
}

bool ServerSocket::GetMessage(std::string& message) {
    char buffer[BufferSize];
    bzero(buffer,BufferSize);
    int n = read(client_sockfd_, buffer, BufferSize - 1);
    if (n <= 0) {
        std::cout << "[WARNING] Socket disconnected!" << std::endl;
        AcceptConnection();
        return false;
    }
    message = std::string(buffer);
    return true;
}

bool ServerSocket::SendMessage(const std::string& message) {
    if (!fd_is_valid(sockfd_)) {
        std::cout << "[ERROR] Invalid socket file descriptor!" << std::endl;
        return false;
    }
    int n = write(client_sockfd_, message.c_str(), message.size());
    if (n <= 0) {
        std::cout << "[WARNING] Socket disconnected!" << std::endl;
        AcceptConnection();
        return false;
    }
    return true;
}

ServerSocket::~ServerSocket() {
    close(client_sockfd_);
    close(sockfd_);
}

ClientSocket::ClientSocket(const std::string& hostname, int port) {
    signal(SIGPIPE, SIG_IGN);
    port_ = port;
    server_info_ = gethostbyname(hostname.c_str());
    if (server_info_ == nullptr) {
        std::cout << "[ERROR] Failed to get server info!" << std::endl;
        return;
    }
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) {
        std::cout << "[ERROR] Failed to open socket!" << std::endl;
        return;
    }
    bzero((char *)&server_address_, sizeof(server_address_));
    server_address_.sin_family = AF_INET;
    bcopy((char *)server_info_->h_addr,
          (char *)&server_address_.sin_addr.s_addr,
          server_info_->h_length);
    server_address_.sin_port = htons(port_);
    if (connect(sockfd_, (struct sockaddr *) &(server_address_), sizeof(server_address_)) < 0) {
        std::cout << "[WARNING] Socket disconnected!" << std::endl;
        return;
    }
}

int ClientSocket::get_sockfd() const {
    return sockfd_;
}

bool ClientSocket::GetMessage(std::string& message) const {
    char buffer[BufferSize];
    bzero(buffer,BufferSize);
    int n = read(sockfd_, buffer, BufferSize - 1);
    if (n < 0) {
        std::cout << "[WARNING] Socket disconnected!" << std::endl;
        return false;
    }
    message = std::string(buffer);
    return true;
}

bool ClientSocket::SendMessage(const std::string& message) const {
    if (!fd_is_valid(sockfd_)) {
        std::cout << "[ERROR] Invalid socket file descriptor!" << std::endl;
        return false;
    }
    int n = write(sockfd_, message.c_str(), message.size());
    if (n < 0) {
        std::cout << "[WARNING] Socket disconnected!" << std::endl;
        return false;
    }
    return true;
}

ClientSocket::~ClientSocket() {
    close(sockfd_);
}