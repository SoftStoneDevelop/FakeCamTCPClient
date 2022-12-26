#pragma once

#include <iostream>
#include <iomanip>
#include <string>

#include "ArrayPool.hpp"
#include "MemoryOwnerFactory.hpp"
#include "TCPClient.hpp"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Must two parameters(now is %d): first IP and second PORT\n", argc);
        return EXIT_FAILURE;
    }

    printf("IP: %s\n",argv[1]);
    printf("PORT: %s\n", argv[2]);

    auto pool = std::make_shared<ArrayPool::ArrayPool<char>>();
    ArrayPool::MemoryOwnerFactory<char> mof(std::move(pool));
    FakeCamClient::TCPClient client(
        argv[1],
        argv[2],
        mof
    );

    while (!client.Connect())
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    auto responce = mof.rentMemory(100);
    std::string command;
    int commandSize = 0;
    while (true)
    {
        std::cout << "Enter command:" << std::endl;
        std::getline(std::cin, command);
        if(command == "Exit")
        {
            break;
        }

        client.sendCommand(command.data(), command.size(), responce);
        std::cout << responce.data();
    }

    return EXIT_SUCCESS;
}
