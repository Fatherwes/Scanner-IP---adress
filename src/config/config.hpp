#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>


#include <WinSock2.h>
#include <WS2tcpip.h>

typedef struct {
    std::string IP;
    int port = 0;
    bool isOpen = false;
    int responseTime = 0;
    double distance = 0;
}ScanResult;

typedef struct{
    WSADATA w_data;
    //socket_CLIENT инициализация переменной для прнимания подключения клиента.
    std::vector <SOCKET> socket_CLIENT, ListenSocket, connectSOCKET;
    sockaddr_in serv_ADDR[2], client_ADDR [2];     
    int ClientADDRlen {sizeof(client_ADDR[0])};

}socket_config;

typedef struct{
    int byteReceived_ONE; 
    int byteReceived_TWO;
}byte;