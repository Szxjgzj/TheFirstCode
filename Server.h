#pragma once
#include <iostream>
#include "WinSock2.h"
#include <thread>
#pragma comment(lib,"ws2_32.lib")

class Server
{
public:
    Server()
    {
        ServerStartup(ServerSocket);
    }
    
    bool ServerStartup(SOCKET& ServerSocket);
    bool OnListening();

    void func(SOCKET& Socket_Client);
public:

    SOCKET ServerSocket;
    SOCKET SocketClients[100];
    int ClientNum = 0;

    
};
