/*
 * @Author: Jie
 * @LastEditors: Jie
 * @FilePath: /src/lib/assist.cpp
 */

#include "assist.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void ListIp() {
    struct ifaddrs * ifAddrStruct = nullptr;
    struct ifaddrs * ifa = nullptr;
    void * tmpAddrPtr = nullptr;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IPv4
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        }
        else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IPv6
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=nullptr)
        freeifaddrs(ifAddrStruct);
}

std::vector<std::string> SplitString(const std::string &s, const std::string &delimiters) {
    std::vector<std::string> tokens;
    std::string::size_type lastPos=s.find_first_not_of(delimiters, 0);
    std::string::size_type pos=s.find_first_of(delimiters, lastPos);
    while(lastPos!=std::string::npos || pos!=std::string::npos) {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
    return tokens;
}

void Error(const char *message) {
    perror(message);
    exit(1);
}