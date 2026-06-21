#include <winsock2.h>
#include <windows.h>
#include <iostream>

#include <thread>
#include <vector>

#include <sstream>
#include <iomanip>
#include <fstream>

#include <cmath>
#include <ctime>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

typedef struct {
    std::string ip;
    int port = 0;
    bool isOpen = false;
    int responseTime = 0;
    double distance = 0;
}ScanResult;


/*Определения дистанции по формуле среднего значения*/
double estimateDistance(const std::string& ip) {

    /*Метод последовательного перебора последнего октета
    из IP   адресса в строковом формате.*/

    int lastOctet = 0;
    for (int i = ip.length() - 1; i >= 0 && ip[i] != '.'; i--) {
        lastOctet = lastOctet * 10 + (ip[i] - '0');
        /* 0 = 0 *10 + (ip[i] - ' 0 ') */
    }

    if (lastOctet <= 50){ 
        return 10.0 + (lastOctet - 1) * 0.8;}

    else if (lastOctet <= 100){
        return 50.0 + (lastOctet - 51) * 1.0;}

    else if (lastOctet <= 150){
        return 100.0 + (lastOctet - 101) * 1.0;}

    else{
        return 150.0 + (lastOctet - 151) * 0.6;}
}



// Передаём указатель, чтобы менять элемент в vector
void scanIP(std::string ip, int port, ScanResult* result){
    result->ip = ip;
    result->port = port;
    result->isOpen = false;
    result->responseTime = 0;
    result->distance = estimateDistance(ip);
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET){
        return;
    }
    
    u_long flags = 1;
    ioctlsocket(sock, FIONBIO, &flags);
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    auto t1 = std::clock();
    int res = connect(sock, (sockaddr*)&addr, sizeof(addr));
    auto t2 = std::clock();
    
    result->responseTime = (t2 - t1) * 1000 / CLOCKS_PER_SEC;
    
    if (res == 0 || WSAGetLastError() == WSAEWOULDBLOCK) {
        result->isOpen = true;
    }
    
    closesocket(sock);
}

void printResult(const ScanResult& check) {
    if (!check.isOpen){
        return;
    }
    
    // Выравнивание IP по ширине
    std::cout << std::left;
    std::cout << std::setw(18) << check.ip;
    std::cout << std::setw(8) << check.port;
    std::cout << std::setw(10) << check.responseTime << " ms  ";
    std::cout << std::fixed << std::setprecision(1) << std::setw(6) << check.distance << " m";
    
    if (check.distance < 50) {
        std::cout << " [Blisko]";
    } else if (check.distance < 100) {
        std::cout << " [Sredne]";
    } else {
        std::cout << " [Dalno]";
    }
    
    std::cout << "\n";
}

std::string toCSV(const ScanResult& check) {
    return check.ip + "," + std::to_string(check.port) + ",open," + 
            std::to_string(check.responseTime) + "," + 
            std::to_string(check.distance);
}

int main() {
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
    
    std::cout << "=" << std::string(80, '=') << "\n";
    std::cout << "IP SCANNER - LOCAL NETWORK SCAN (Port 80, 443, 8080)\n";
    std::cout << "=" << std::string(80, '=') << "\n\n";
    
    std::vector<std::thread> threads;
    std::vector<ScanResult> results;
    const std::string base = "192.168.1.";
    std::vector<int> ports = {80, 443, 8080};
    
    std::cout << "Scanning network: " << base << "1-254\n";
    std::cout << "Ports: 80 (HTTP), 443 (HTTPS), 8080 (Alt HTTP)\n\n";
    
    auto startTime = std::clock();
    
    // Создаём все результаты заранее
    for (int i = 1; i <= 254; ++i) {
        for (int port : ports) {
            ScanResult result;
            result.ip = base + std::to_string(i);
            result.port = port;
            result.isOpen = false;
            result.responseTime = 0;
            result.distance = estimateDistance(result.ip);
            results.push_back(result); 
        }
    }
    
    // Запускаем threads с указателем на элемент vector
    for (size_t idx = 0; idx < results.size(); idx++) {
        threads.emplace_back([idx, &results]() {
            // Передаём указатель на результат
            scanIP(results[idx].ip, results[idx].port, &results[idx]);
        });
    }
    
    for (auto& t : threads) t.join();
    
    auto endTime = std::clock();
    double totalTime = (endTime - startTime) * 1000 / CLOCKS_PER_SEC;
    
    std::cout << "=" << std::string(80, '=') << "\n";
    std::cout << "                    FOUND OPEN PORTS (sorted by distance)\n";
    std::cout << "=" << std::string(80, '=') << "\n\n";
    
    std::sort(results.begin(), results.end(), 
        [&](const ScanResult& a, const ScanResult& b){
            return a.distance < b.distance;});
    
    std::cout << std::left;
    std::cout << std::setw(18) << "IP ADDRESS";
    std::cout << std::setw(8) << "PORT";
    std::cout << std::setw(10) << "TIME(ms)";
    std::cout << std::setw(10) << "DISTANCE";
    std::cout << "STATUS\n";
    std::cout << std::string(80, '-') << "\n";
    
    int count = 0;
    for (const auto& r : results) {
        if (r.isOpen) {
            printResult(r);
            count++;
        }
    }
    
    std::cout << std::string(80, '-') << "\n";
    std::cout << "\nTotal open ports: " << count << "\n";
    std::cout << "Scan completed in: " << std::fixed << std::setprecision(2) << totalTime << " seconds\n";
    std::cout << "=" << std::string(80, '=') << "\n";
    
    std::ofstream csv("scan_results.csv");
    csv << "IP,Port,Status,ResponseTime(ms),Distance(m)\n";
    for (const auto& r : results) {
        if (r.isOpen) csv << toCSV(r) << "\n";
    }
    csv.close();
    
    std::cout << "\nResults saved to: scan_results.csv\n";
    std::cout << "=" << std::string(80, '=') << "\n";
    
    WSACleanup();
    return 0;
}