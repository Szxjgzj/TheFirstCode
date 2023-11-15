#include <iostream>
#include "Server.h"

int main(int argc, char* argv[])
{
    Server* Server = new ::Server();

    
    while (true)
    {
        Server->OnListening();
    }
    
    return 0;
}


