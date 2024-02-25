#include <iostream>
#pragma comment (lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#pragma warning(disable: 4996)

using namespace std;

int main()
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
        cout << "Error with loading library" << endl;
        exit(1);
    }

    SOCKADDR_IN ad;
    int sizeooflocal = sizeof(ad);
    ad.sin_addr.s_addr = inet_addr("127.0.0.1");
    ad.sin_port = htons(1228);
    ad.sin_family = AF_INET;

    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
    bind(s, (SOCKADDR*)&ad, sizeooflocal);

    char buffer[1024];
    int len;
    SOCKADDR_IN client;
    int sizeoofclient = sizeof(client);

    while (true) {

        len = recvfrom(s, buffer, sizeof(buffer), 0, (SOCKADDR*)&client, &sizeoofclient);
        if (len == SOCKET_ERROR) {
            cout << "Error with receiving data" << endl;
            break;
        }
        buffer[len] = '\0';
        cout << "Received string: " << buffer << endl;

        char* readPtr = buffer;
        char* writePtr = buffer;
        int count = 0; 

        while (*readPtr) {
            char* endPtr;
            long num = strtol(readPtr, &endPtr, 10);
            if (endPtr != readPtr) {
                if (num % 4 != 0) {
                    while (readPtr != endPtr) {
                        *(writePtr++) = *(readPtr++);
                    }
                }
                else {
                    readPtr = endPtr;
                    count++; 
                }
            }
            else {
                *(writePtr++) = *(readPtr++);
            }
        }

        *writePtr = '\0';

        buffer[strlen(buffer)] = '\0'; 

        cout << "Transformed string: " << buffer << endl;
        cout << "Number of deleted numbers: " << count << endl; 


        char message[1024];
        sprintf(message, "%s | %d", buffer, count); 
        if (sendto(s, message, strlen(message), 0, (SOCKADDR*)&client, sizeoofclient) == SOCKET_ERROR) {
            cout << "Error with sending data" << endl;
            break;
        }
        cout << "Sent message: " << message << endl;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}