#include <iostream>
#include "base.h"
#include "manager.h"
int main(int argc, char** argv)
{
    if (argc >= 2 && std::string{ argv[1] } == "-h")
    {
        Utils::help();
        return 0;
    }
    try {
        while (1) {
            std::cout << "xxcc@xxcc-toy-database:~$ ";
            std::string command;
            std::getline(std::cin, command);
            if (command == "exit()") break;
            else Manager::getInstance(".\\config.conf").execute(command);
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}