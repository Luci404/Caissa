#include "CaissaBoard.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <iterator>

CaissaBoard board = CaissaBoard(8, 8);

std::vector<std::string> SplitString(std::string str, std::string token){
    std::vector<std::string>result;
    while(str.size())
    {
        int index = str.find(token);
        if(index != std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

void CommandLoop()
{
    // Get command.
    std::string commandString;
    std::cout << ">>> ";
    std::getline(std::cin, commandString);
    std::cout << "COMMAND: " << commandString << std::endl;

    // Get command componenets.
    std::vector<std::string> commandComponents = SplitString(commandString, " ");

    // Handle command.
    if (commandComponents.size() > 0)
    {
        if (commandComponents[0] == "HELP")
        {
            std::cout << std::setw(8) << std::left << "HELP" << std::setw(128) << std::left << "Provides a list of commands and an overview of their function." << std::endl;
            std::cout << std::setw(8) << std::left << "PRINT" << std::setw(128) << std::left << "Writes the board to the console." << std::endl;
        
        }
        else if (commandComponents[0] == "PRINT") 
        {
            for (size_t rank = 0; rank < board.GetRankCount(); rank++)
            {
                for (size_t file = 0; file < board.GetFileCount(); file++)
                {
                    std::cout << "x" << " ";
                }
                std::cout << std::endl;
            }
        }
        else
        {
           std::cout << "Unknown command: " << commandComponents[0] << std::endl; 
        }
    }

    // Repeat...
    CommandLoop();
}

int main(int argc, char *argv[])
{
    std::cout << "Caissa 1.0.0" << std::endl;

    CommandLoop();
}