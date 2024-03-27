#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <vector>
#include <array>
#include <string>
#include <iomanip>

#pragma warning(disable:4996)

using namespace std;

vector<array<string, 5>> information = { {"Иванов Иван Иванович","568010","28","168","86"},
											 {"Горин Геннадий Александрович","875078","31","181","96"},
											 {"Жидковский Алексей Валерьевич","372304","30","177","100"},
											 {"Достоевский Фкдор Михайлович","8106544","25","173","89"},
											 {"Агапов Алексей Михайлович","212777","30","178","86"} };


DWORD WINAPI ThreadFunc(LPVOID client_socket) {
	SOCKET s2;
	s2 = ((SOCKET*)client_socket)[0];
	int choice_person;
	char buf[2];
	char buf1[100];
	char buf2[100];
	array<string, 5> arr;
	string inf;
	int person = 0;
	int parametr = 0;
	string value;
	while (true) {
		recv(s2, buf, sizeof(buf), 0);
		int choice = atoi(buf);
		switch (choice) {
		case 1: case 3: case 4:
			send(s2, to_string(size(information)).c_str(), sizeof(to_string(size(information)).c_str()), 0);//отправить размер вектора information
			for (int i = 0; i < information.size(); i++) {
				inf += to_string(i + 1) + ". " + information[i][0] + " (" + information[i][1] + "): " + information[i][2] + " лет, " + information[i][3] + " см, " + information[i][4] + " кг\n";
			}
			cout << inf << "    " << inf.length() << endl;
			send(s2, inf.c_str(), inf.length() + 1, 0);//отправка этой строки
			inf.clear();
			if (choice == 3) {
				recv(s2, buf1, sizeof(buf1), 0);
				person = atoi(buf1);
				memset(buf1, 0, sizeof(buf1));
				recv(s2, buf1, sizeof(buf1), 0);
				parametr = atoi(buf1);
				memset(buf1, 0, sizeof(buf1));
				int bute = recv(s2, buf1, sizeof(buf1), 0);
				if (strlen(buf1) != 0) {
					buf1[bute] = '\0';
					value = buf1;
					memset(buf1, 0, sizeof(buf1));
				}
				information[person - 1][parametr - 1] = value;
			}
			if (choice == 4) {
				recv(s2, buf, sizeof(buf), 0);
				person = atoi(buf);
				information.erase(information.cbegin() + person - 1);
			}
			break;
		case 2:
			for (int i = 0; i <= 5; i++) {
				int bute = recv(s2, buf2, sizeof(buf2), 0);
				if (bute != 0) {
					if (strlen(buf2) != 0) {
						buf2[bute] = '\0';
						//cout << sizeof(buf2) << buf2 << endl;
						arr[i - 1] = buf2;
						memset(buf2, 0, sizeof(buf2));
					}
				}
				else cout << "error!!";
			}
			information.push_back(arr);
			break;
		case 5:
			person = 0;
			parametr = stoi(information[0][3] + information[0][4]);
			for (int i = 1; i < information.size(); i++) {
				int t = stoi(information[i][3] + information[i][4]);
				if (parametr > t) {
					parametr = t;
					person = i;
				}
			}
			inf = information[person][0] + "\n";
			send(s2, inf.c_str(), inf.length() + 1, 0);
			inf.clear();
			break;
		}
		memset(buf, 0, sizeof(buf));
	}
	closesocket(s2);
}

void main() {
	setlocale(LC_ALL, "rus");
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);

	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;

	bind(s, (sockaddr*)&local_addr, sizeof(local_addr));
	int c = listen(s, SOMAXCONN);

	cout << "Server receive ready" << endl;
	cout << endl;
	SOCKET client_socket; // сокет для клиента 
	sockaddr_in client_addr; // адрес клиента
	int client_addr_size = sizeof(client_addr);
	//char** information;
	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))) {
		DWORD thID;// thID идентификатор типа DWORD 
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
		ExitThread(thID);
	}
}