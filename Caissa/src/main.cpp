#include "CaissaBoard.h"
#include "CaissaSquare.h"
#include "CaissaPiece.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <iterator>

CaissaBoard board = CaissaBoard(0, 0);

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
            std::cout << std::setw(16) << std::left << "HELP" << std::setw(128) << std::left << "Provides a list of commands and an overview of their function." << std::endl;
            std::cout << std::setw(16) << std::left << "PRINT" << std::setw(128) << std::left << "Writes the board to the console." << std::endl;
            std::cout << std::setw(16) << std::left << "MOVE <UCI>" << std::setw(128) << std::left << "Performs a move based on a UCI string." << std::endl;
        }
        else if (commandComponents[0] == "PRINT") 
        {
            for (size_t rank = 0; rank < board.GetRankCount(); rank++)
            {
                for (size_t file = 0; file < board.GetFileCount(); file++)
                {
                    CaissaSquare* square = board.GetSquare(rank, file);
                    if (square == nullptr) continue;
                    std::cout << (square->Occupied() ? "1" : "0") << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (commandComponents[0] == "MOVE")
        {
            if (commandComponents.size() > 1 && commandComponents[1].size() == 4)
            {
                uint16_t originRank = std::string("abcdefghijklmnopqrstuvwxyz").find(commandComponents[1][0]);
                uint16_t originFile = std::string("123456789").find(commandComponents[1][1]);
                uint16_t targetRank = std::string("abcdefghijklmnopqrstuvwxyz").find(commandComponents[1][2]);
                uint16_t targetFile = std::string("123456789").find(commandComponents[1][3]);

                if (!board.Move(originRank, originFile, targetRank, targetFile))
                {
                    std::cout << "Failed to perform move: (" << originRank << ", " << originFile << ") -> (" << targetRank << ", " << targetFile << ")";
                }
            }
            else
            {
                std::cout << "The syntax of the command is incorrect." << std::endl;
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
    board = CaissaBoard(8, 8);

    // Init
    board.AddPiece(CaissaPiece(), 1, 1);

    std::cout << "Caissa 1.0.0" << std::endl;

    CommandLoop();
}