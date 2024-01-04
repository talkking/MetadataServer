/*
 * @Author: Jie
 * @LastEditors: Jie
 * @FilePath: /include/communication.hpp
 */

#ifndef METADATA_MANAGEMENT_COMMUNICATION_H
#define METADATA_MANAGEMENT_COMMUNICATION_H

#include "assist.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <string>


#define BufferSize 1024


class ServerSocket {
public:
    ServerSocket(int port);
    int AcceptConnection();
    std::string get_client_address_string_();
    bool GetMessage(std::string& message);
    bool SendMessage(const std::string& message);
    ~ServerSocket();

private:
    int sockfd_;
    int port_;
    int client_sockfd_;
    sockaddr_in server_address_, client_address_;
    sockaddr *p_client_address_;
    socklen_t client_address_length_;
    std::string client_address_string_;
};


class ClientSocket {
public:
    ClientSocket(const std::string& hostname, int port);
    int get_sockfd() const;
    bool GetMessage(std::string& message) const;
    bool SendMessage(const std::string& message) const;
    ~ClientSocket();

private:
    int sockfd_;
    int port_;
    struct sockaddr_in server_address_;
    struct hostent *server_info_;
};

#endif //METADATA_MANAGEMENT_COMMUNICATION_H
