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

	SOCKADDR_IN add;
	add.sin_addr.s_addr = inet_addr("127.0.0.1");
	add.sin_port = htons(1228);
	add.sin_family = AF_INET;

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	char buffer[1024]; 
	int len; 
	char message[1024]; 
	SOCKADDR_IN server; 
	int sizeoofserver = sizeof(server); 

	while (true) {
		cout << "Enter a string: ";
		cin.getline(buffer, sizeof(buffer));
		len = strlen(buffer);

		if (len % 4 != 0) {
			cout << "The string is not divisible by 4" << endl;
			continue; 
		}

		if (sendto(s, buffer, len, 0, (SOCKADDR*)&add, sizeoofserver) == SOCKET_ERROR) {
			cout << "Error with sending data" << endl;
			break;
		}
		cout << "Sent string: " << buffer << endl;

		len = recvfrom(s, message, sizeof(message), 0, (SOCKADDR*)&server, &sizeoofserver);
		if (len == SOCKET_ERROR) {
			cout << "Error with receiving data" << endl;
			break;
		}
		message[len] = '\0';
		cout << "Received message and number of deleted numbers: " << message << endl;
	}

	closesocket(s);
	WSACleanup();
	return 0;
}