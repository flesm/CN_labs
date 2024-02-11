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

	SOCKADDR_IN peer; // стварэнне структуры для захоўвання адрасу сокета
	//int sizeooflocal = sizeof(local);
	peer.sin_addr.s_addr = inet_addr("127.0.0.1"); // заданне IP-адрасу як лакальны
	peer.sin_port = htons(1228); // заданне парту сокета як 1228 (пажадана 1024-4999)
	peer.sin_family = AF_INET;

	SOCKET connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(connection, (SOCKADDR*)&peer, sizeof(peer)) != 0) {
		cout << "Error with connection to server.\n";
		return 1;
	}
	cout << "Connected!\n";
	
	int number; // лік, уведзены карыстальнікам
	int count; // колькасць лікаў, кратных 3-ом, атрыманых ад сервера

	do {
		cout << "Enter a number (0 or other to stop): ";
		cin >> number;
		send(connection, (char*)&number, sizeof(number), NULL); // адпраўляем лік серверу
	} while (number != 0); 

	recv(connection, (char*)&count, sizeof(count), NULL); // атрымліваем ад сервера патрэбны лік і прывязваем да пераменнай count
	cout << "The number of multiples of three is: " << count << endl; 

	closesocket(connection);
	WSACleanup();
	return 0;
}
