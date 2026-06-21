#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <mutex>
#include <chrono>
#include <future>
#include <thread>
#include <queue>

std::mutex mtx;

void task_async_1(){
    mtx.lock();  
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "===============| Started func |===============" << std::endl;
    std::cout << " Work ... func one" << std::endl;
    std::cout << "===============| Stoped func |===============" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    mtx.unlock();
}


void task_async_2(){
    mtx.lock();  
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "===============| Started func |===============" << std::endl;
    std::cout << " Work ... func two" << std::endl;
    std::cout << "===============| Stoped func |===============" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    mtx.unlock();
}

int task_async_3(){
    
}

float task_async_4(){
     

}

void task_thread(){
    

}