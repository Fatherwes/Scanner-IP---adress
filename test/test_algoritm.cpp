#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <stdexcept>

#include <mutex>
#include <chrono>
#include <future>
#include <thread>
#include <queue>

std::mutex mtx;


template  <typename T>
class Stack{  
    private:
        
    public:
        void push{

        }

    
};




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

int task_async_3(int *a, int *b){
    
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "===============| Started math |==============" << std::endl;
    int result = *a + (*b);
    for( int i = 0; i <= result; ++i){
        std::cout << "Test for " << i + 1 << std::endl;
    }
    std::cout << "Number task" << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "===============| Stoped math |==============" << std::endl;
    return 1;
}

float task_async_4(float *a, float *b){
    std::lock_guard <std::mutex> lock(mtx);
    std::cout << "===============| Started math |==============" << std::endl;
    float result = *a + (*b);
    for(int i = 0; i <= result; ++i){
        std::cout << "Test for type data FLOAT: " << 0.0f + ((float)i);
    }
    std::cout << "Number task" << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "===============| Stoped math |==============" << std::endl;

    return -1;
}

void task_thread(){
     

}








int main(){
 
    
}
