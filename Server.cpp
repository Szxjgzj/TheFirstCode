#include "Server.h"
bool Server::ServerStartup(SOCKET& ServerSocket)
{
    // 启动网络库 WinSock相关函数才能使用
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed" << std::endl;
        return false;
    }
    // 创建 Socket，返回Socket句柄
    // SOCKET：Socket句柄
    // AF_INET：IPV4
    // SOCK_STREAM：流式传输
    // IPPROTO_TCP：使用TCP协议
    ServerSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if ( ServerSocket == SOCKET_ERROR)
    {
        std::cout << "Create Socket Error" << std::endl;
        WSACleanup();
        closesocket(ServerSocket);
        return false;
    }
    // Socket地址结构体（IPV4，IP，端口），服务端即用本地IP地址和一个未被占用的端口去设置
    sockaddr_in Addr;
    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(8080);
    Addr.sin_addr.S_un.S_addr = INADDR_ANY;
    // Socket绑定到这个IP地址上
    if (bind(ServerSocket,(sockaddr*)&Addr,sizeof(Addr)) == SOCKET_ERROR)
    {
        std::cout << "Bind Error" << std::endl;
        WSACleanup();
        closesocket(ServerSocket);
        return false;
    }
    if (listen(ServerSocket,1024) == SOCKET_ERROR)
    {
        std::cout << "Listen Error" << std::endl;
        WSACleanup();
        closesocket(ServerSocket);
        return false;
    }
    std::cout << "is Listening" << std::endl;
    return true;
}

bool Server::OnListening()
{
    SocketClients[ClientNum] = accept(ServerSocket,nullptr,nullptr);
    if (SocketClients[ClientNum] == SOCKET_ERROR)
    {
        std::cout << "Accpet Error" << std::endl;
        return false;
    }
    int CurrentIndex = ClientNum;
    ClientNum++;
    std::cout << "A Client Join!" << std::endl;
    std::thread thread(&Server::func,this,std::ref(SocketClients[CurrentIndex]));
    thread.detach();
    return true;
}

void Server::func(SOCKET& Socket_Client)
{
    int len = 1;
    while (len > 0)
    {
        //std::thread::id this_id = std::this_thread::get_id();
        //std::cout << "thread is is " << this_id << std::endl;
        char buffer[1024] = {0};
        len = recv(Socket_Client,buffer,sizeof(buffer),0);
        std::cout << "len is " << len << std::endl;
        if (len > 0)
        {
            std::cout << "recv is from " << buffer << std::endl;
            send(Socket_Client , buffer, (int)len , 0);
        }
    }
    return;
}

