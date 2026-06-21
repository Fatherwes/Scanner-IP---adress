#include <iostream>
#include <stdint.h>
#include <stdio.h> 
#include <stdlib.h>

#include <mutex> 
#include <thread>
#include <chrono>
#include <future>

#include <vector>
#include <functional>

#include "include/func_scanner.hpp"
#include "config/config.hpp"

std::mutex mtx;

void  ___socket_callback(){

}

void  ___task_callback(){
    std::future <int> per = std::async();
}

/*Сканнер IP - адрессов и портов*/
int main(){
    socket_config sock;
    if(WSAStartup(MAKEWORD(2,2), &sock.w_data) != 0){
        printf("Listening failed \n");
        WSACleanup();
        throw std::runtime_error("Failed \n");
    }
    
    std::cout <<
} 
