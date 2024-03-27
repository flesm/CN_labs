#include <iostream>
#pragma comment (lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable: 4996)

using namespace std;

int main()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2); // версія бібліятэкі
	if (WSAStartup(wVersionRequested, &wsaData) != 0) { // праверка
		cout << "Error with loading library" << endl;
		exit(1);
	}

	SOCKADDR_IN local; // стварэнне структуры для захоўвання адрасу сокета
	int sizeooflocal = sizeof(local);
	local.sin_addr.s_addr = inet_addr("127.0.0.1"); // заданне IP-адрасу як лакальны
	local.sin_port = htons(1228); // заданне парту сокета як 1228 (пажадана 1024-4999)
	local.sin_family = AF_INET; // заданне сямейства адрасоў сокета як AF_INET (канстанта для сямейства інтэрнэт-пратаколаў)

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0); // стварэнне сокета (2, 3 - тып ствараемага сокета і пратакол абмену дадзенымі праз яго) 
	bind(s, (SOCKADDR*)&local, sizeooflocal); // функцыя для звязвання адрасу з сокетам

	listen(s, SOMAXCONN); // функцыя прызначаная для праслухоўвання уваходзячых падключэнняў на сокет (SOMAXCONN - 0x7FFFFFFF падключэнняў)

	SOCKET newConnection;
	newConnection = accept(s, (SOCKADDR*)&local, &sizeooflocal);

	if (newConnection == 0) {
		cout << "Error with connection to server\n";
	}
	else {
		cout << "Client connected!\n";
		
		int number; // лік, атрыманы ад кліента 
		int count = -1; // колькасць лікаў, кратных 3-ом

		do {
			recv(newConnection, (char*)&number, sizeof(number), NULL); // атрымліваем лік ад кліента і прывязваем да нашай пераменнай number
			if (number % 3 == 0) { 
				count++; 
			}
		} while (number != 0); 

		send(newConnection, (char*)&count, sizeof(count), NULL); // адпраўляем атрыманую колькасць лікаў, кратных 3-ом, кліенту
	}

	closesocket(newConnection);
	closesocket(s); 
	WSACleanup();
	return 0;
}

