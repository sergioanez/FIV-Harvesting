// UPDServerWindows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(int argc, char*argv[])
{
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        cout << "Can;t start Winsock! " << wsOk;
        return;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    string s(argv[1]);
    int sendOk= sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    if (sendOk == SOCKET_ERROR)
    {
        cout << "That didn't work! " << WSAGetLastError() << endl;
    }


    closesocket(out);
    WSACleanup();
}


