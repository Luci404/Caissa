#include <iostream>
#include <bits/stdc++.h>
#include "Caissa.h"

Caissa::StandardBoard board;

std::vector<std::string> SplitString(std::string str, std::string token)
{
    std::vector<std::string> result;
    while (str.size())
    {
        int index = str.find(token);
        if (index != std::string::npos)
        {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)
                result.push_back(str);
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
    std::transform(commandString.begin(), commandString.end(), commandString.begin(), ::toupper);

    // Get command componenets.
    std::vector<std::string> commandComponents = SplitString(commandString, " ");

    // Handle command.
    if (commandComponents.size() > 0)
    {
        if (commandComponents[0] == "HELP") 
        {
            std::cout << "HELP              " << "Provides a list of commands and an overview of their function." << std::endl;
            std::cout << "PRINT             " << "Writes the board to the console." << std::endl;
            std::cout << "MOVE <UCI>        " << "Performs a move based on a UCI string." << std::endl;
            std::cout << "PERFT <DEPTH>     " << "Count all the leaf nodes of a certain depth." << std::endl;
            std::cout << "LEGALMOVES        " << "Print all legal moves." << std::endl;
            std::cout << "FEN               " << "Print the current board position in FEN notation." << std::endl;
        }
        else if (commandComponents[0] == "PRINT")
        {
            board.Print();
        }
        else if (commandComponents[0] == "MOVE")
        {
            if (commandComponents.size() > 1 && commandComponents[1].size() == 4)
            {
                uint16_t originFile = std::string("abcdefghijklmnopqrstuvwxyz").find(std::tolower(commandComponents[1][0]));
                uint16_t originRank = std::string("123456789").find(commandComponents[1][1]);
                uint16_t targetFile = std::string("abcdefghijklmnopqrstuvwxyz").find(std::tolower(commandComponents[1][2]));
                uint16_t targetRank = std::string("123456789").find(commandComponents[1][3]);
                uint16_t originIndex = originRank * 8 + originFile;
                uint16_t targetIndex = targetRank * 8 + targetFile;

                Caissa::Move move = Caissa::Move(originIndex, targetIndex, board.pieces[targetIndex]);
                board.MakeMove(move);
            }
            else
            {
                std::cout << "The syntax of the command is incorrect." << std::endl;
            }
        }
        else if (commandComponents[0] == "PERFT")
        {
            if (commandComponents.size() > 1)
            {
                uint16_t depth = std::stoi(commandComponents[1]);
                std::cout << "Perft(" << depth << "): " << board.Perft(depth) << std::endl;
            }
            else
            {
                std::cout << "The syntax of the command is incorrect." << std::endl;
            }
        }
        else if (commandComponents[0] == "LEGALMOVES")
        {
            std::vector<Caissa::Move> moves = board.GetLegalMoves();
            for (Caissa::Move move : moves)
            {
                std::string UCI = "0000";
                UCI[0] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.OriginSquare)];
                UCI[1] = std::string("123456789")[ROW(move.OriginSquare)];
                UCI[2] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.TargetSquare)];
                UCI[3] = std::string("123456789")[ROW(move.TargetSquare)];

                std::cout << UCI << std::endl;
            }
        }
        else if (commandComponents[0] == "FEN")
        {
            std::cout << board.Fen() << std::endl;
        }
        else
        {
            std::cout << "Unknown command: " << commandComponents[0] << std::endl;
        }
    }

    // Repeat...
    CommandLoop();
}

/* Application */
int main(int argc, char *argv[])
{
    board = Caissa::StandardBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    std::cout << "Caissa 1.0.0" << std::endl;

    CommandLoop();
}