#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <functional>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iomanip>
#include <c++/15.2.0/algorithm>
#include "config.hpp"


namespace func{
    
    void ___scan_Ip_callback__(std::string ip, int16_t port, ScanResult* result){
        result->IP = ip;
        result->port = port;
        result->isOpen = 0;
        result->responseTime = 0;
        result->distance;

        u_long flags = 1;
        socket_config sock;
        sock.connectSOCKET[0] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock.connectSOCKET[0] == INVALID_SOCKET){
            return;
        }
           
        ioctlsocket(sock.connectSOCKET[0], FIONBIO, &flags);
        
        /*Задаем парамметры сокета для 
        дальнейшей работы*/
        sock.serv_ADDR[0].sin_family = AF_INET;
        sock.serv_ADDR[0].sin_port =  htons(port);
        sock.serv_ADDR[0].sin_addr.S_un.S_addr = inet_addr(ip.c_str());
        
        auto clock_res = [&](){
            auto time_clock_one = std::clock();
            int res = connect(sock.connectSOCKET[0],(sockaddr*)&sock.serv_ADDR, sizeof(sock.serv_ADDR));
            auto time_clock_two = std::clock();
            
            result->responseTime = (time_clock_two - time_clock_one) * 100 / CLOCKS_PER_SEC;
            if(res == 0 || WSAGetLastError() == WSAEWOULDBLOCK){
                result->isOpen = true;
                printf("Error");
            }
        };
        
        clock_res();
        closesocket(sock.connectSOCKET[0]);
    }
    
    void printResult(const ScanResult& check){
        if(!check.isOpen){
            return;
        }

        std::cout << std::left;
        std::cout << std::setw(18) <<check.IP;
        std::cout << std::setw(8) <<check.port;
        std::cout << std::setw(10) << check.responseTime << " ms ";
        std::cout << std::fixed << std::setprecision(1) << std::setw(6) << check.distance << "m";
        
        if(check.distance < 50){
            std::cout << "[Blisko]";}
        
        else if (check.distance < 100){
            std::cout << "[Sredne]";}
        
        else{
            std::cout << "[Dalno]";
        }
        std::cout << '\n';
    }

    std::string toCSV(const ScanResult& check){
        return check.IP + "," + std::to_string(check.port) + ",open," + 
        std::to_string(check.responseTime) + "," +
        std::to_string(check.distance);
    }
    
};

