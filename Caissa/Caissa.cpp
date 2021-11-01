#include <inttypes.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <bits/stdc++.h>

/* TODO: ♙♘♗♖♕♔·♟♞♝♜♛♚ */

/* Board Representation */
typedef uint8_t Piece;

uint16_t mailbox[120] = {
    UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX,
    UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX,
    UINT16_MAX,  0,  1,  2,  3,  4,  5,  6,  7, UINT16_MAX,
    UINT16_MAX,  8,  9, 10, 11, 12, 13, 14, 15, UINT16_MAX,
    UINT16_MAX, 16, 17, 18, 19, 20, 21, 22, 23, UINT16_MAX,
    UINT16_MAX, 24, 25, 26, 27, 28, 29, 30, 31, UINT16_MAX,
    UINT16_MAX, 32, 33, 34, 35, 36, 37, 38, 39, UINT16_MAX,
    UINT16_MAX, 40, 41, 42, 43, 44, 45, 46, 47, UINT16_MAX,
    UINT16_MAX, 48, 49, 50, 51, 52, 53, 54, 55, UINT16_MAX,
    UINT16_MAX, 56, 57, 58, 59, 60, 61, 62, 63, UINT16_MAX,
    UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX,
    UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX
};

uint16_t mailbox64[64] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

#define ROW(x)			(x >> 3)
#define COL(x)			(x & 7)

enum MoveType : uint8_t
{
    NORMAL,
    PROMOTION,
    EN_PASSANT,
    CASTLING
};

struct Move
{
public:
    Move(uint16_t originSquare, uint16_t targetSquare, Piece capturePiece = 0x00, MoveType moveType = MoveType::NORMAL)
        : OriginSquare(originSquare), TargetSquare(targetSquare), CapturePiece(capturePiece), MoveType(moveType)
    {
    }

public:
    const uint16_t OriginSquare;
    const uint16_t TargetSquare;
    const Piece CapturePiece;
    const uint8_t MoveType;
};

class Board 
{
public:
    virtual void Print() const = 0;    
    virtual bool IsCheck() const = 0;
    virtual void MakeMove(Move move) = 0;
    virtual void UndoMove(Move move) = 0;
    virtual std::vector<Move> GetLegalMoves() const = 0;    
};

class StandardBoard : public Board
{
public:
    StandardBoard() 
    {
        // Init
        for (int i = 0; i < 64; i++)
        { 
            // White
            if (i == 0 || i == 7) pieces[i] = 'R';
            else if (i == 1 || i == 6) pieces[i] = 'N';
            else if (i == 2 || i == 5) pieces[i] = 'B';
            else if (i == 3) pieces[i] = 'Q';
            else if (i == 4) pieces[i] = 'K';
            else if (i > 7 && i < 16) pieces[i] = 'P';
            // Black
            else if (i == 7 * 8 + 0 || i == 7 * 8 + 7) pieces[i] = 'r';
            else if (i == 7 * 8 + 1 || i == 7 * 8 + 6) pieces[i] = 'n';
            else if (i == 7 * 8 + 2 || i == 7 * 8 + 5) pieces[i] = 'b';
            else if (i == 7 * 8 + 3) pieces[i] = 'q';
            else if (i == 7 * 8 + 4) pieces[i] = 'k';
            else if (i > 47 && i < 56) pieces[i] = 'p';
            //else if (i > 6 * 8 - 1 && i < 8 * 8 + 0) pieces[i] = 'p';
            else pieces[i] = 0x00; 
        }

        /*for (int i = 0; i < 64; i++)
        {
            if (i == 25) pieces[i] = 'R';
            else pieces[i] = 0x00;
        }*/
    }

    virtual void Print() const override
    {   
        std::cout << "+-- a b c d e f g h --+" << std::endl;
        std::cout << "|                     |" << std::endl;
        for (int16_t rank = 7; rank >= 0; rank--)
        {
            std::cout << rank+1 << "   ";
            for (uint16_t file = 0; file < 8; file++) 
            {
                std::cout << (char)(pieces[rank * 8 + file] == 0x00 ? '-' : pieces[rank * 8 + file]) << " ";
            }
            std::cout << "  " << rank+1 << std::endl;
        }
        std::cout << "|                     |" << std::endl;
        std::cout << "+-- a b c d e f g h --+" << std::endl;
    }

    /*
    Counts all the leaf nodes of a certain depth.
     - https://www.chessprogramming.org/Perft
     - https://www.chessprogramming.org/Perft_Results
    */
    template<bool root>
    uint64_t Perft(int depth)
    {
        uint64_t nodes = 0;
        uint64_t count = 0;

        const bool leaf = (depth == 2);

        for (Move move : GetLegalMoves())
        {
            if (root && depth <= 1)
            {
                count = 1;
                nodes++;
            }
            else
            {
                MakeMove(move);
                count = leaf ? GetLegalMoves().size() : Perft<false>(depth - 1);
                nodes += count;
                UndoMove(move);
            }

            if (root)
            {
                std::string UCI = "0000";
                UCI[0] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.OriginSquare)];
                UCI[1] = std::string("123456789")[ROW(move.OriginSquare)];
                UCI[2] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.TargetSquare)];
                UCI[3] = std::string("123456789")[ROW(move.TargetSquare)];
                std::cout << UCI << ": " << count << std::endl;
            }
        }
        
        return nodes;

        //std::cout << "PERFT" << std::endl;
        /*uint64_t nodes = 0;
        
        std::vector<Move> moves = GetLegalMoves();*/
        /*for (Move move : moves)
        {
            std::string UCI = "0000";
            UCI[0] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.OriginSquare)];
            UCI[1] = std::string("123456789")[ROW(move.OriginSquare)];
            UCI[2] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.TargetSquare)];
            UCI[3] = std::string("123456789")[ROW(move.TargetSquare)];
            std::cout << move.OriginSquare << "->" << move.TargetSquare << " (" << UCI << ")" << (whiteSideToMove ? "white" : "black") <<std::endl;

            MakeMove(move);
            Print();
            UndoMove(move);
        }*/

          /*if (depth <= 1) { return moves.size(); }

        for (Move move : moves)
        {
            MakeMove(move);
            if (!IsCheck())
                nodes += Perft(depth - 1);
            UndoMove(move);
        }

        return nodes;*/
    }

    virtual bool IsCheck() const override
    {
        for (uint16_t i = 0; i < 64; ++i)
        {
            if (std::toupper(pieces[i]) == 'K' &&  std::isupper(pieces[i]) == whiteSideToMove)
            {
                return IsAttacking(i, !whiteSideToMove);
            }
        }

        return true; /* This should never be reached. */
    }

    /*
    Returns true if the specified square is beeing attack by the specified side.
    */
    bool IsAttacking(uint16_t squareIndex, bool side) const
    {
        std::vector<Move> moves = GetLegalMoves();
        for (Move move : moves)
        {
            if (move.TargetSquare == squareIndex && std::isupper(move.OriginSquare) == side)
            {
                return true;
            }
        }

        return false;
    }

    virtual void MakeMove(Move move) override
    {
        whiteSideToMove = std::islower(pieces[move.OriginSquare]);
        pieces[move.TargetSquare] = pieces[move.OriginSquare];
        pieces[move.OriginSquare] = 0x00;
    }

    virtual void UndoMove(Move move) override
    {
        pieces[move.OriginSquare] = pieces[move.TargetSquare];
        pieces[move.TargetSquare] = move.CapturePiece;
        whiteSideToMove = std::isupper(pieces[move.OriginSquare]);
    }

    std::vector<Move> GetLegalMoves() const override
    {
        std::vector<Move> moves;

        for (uint16_t i = 0; i < 64; ++i)
        {
            if (std::isupper(pieces[i]) == whiteSideToMove)
            {
                if (pieces[i] == 'P')
                {
                    //std::cout << i << " - "<< COL(i) << ", " << ROW(i) << std::endl;
                    // Generate white pawn captures.
                    if (COL(i) != 0 && std::islower(pieces[i + 7])) moves.push_back(Move(i, i + 7, pieces[i + 7]));
                    if (COL(i) != 7 && std::islower(pieces[i + 9])) moves.push_back(Move(i, i + 9, pieces[i + 9]));
                    // Generate white pawn push and long push.
                    if (pieces[i + 8] == 0x00)
                    {
                        moves.push_back(Move(i, i + 8, pieces[i + 8]));
                        if (pieces[i + 16] == 0x00 && i <= 15) moves.push_back(Move(i, i + 16, pieces[i + 16]));
                    }
                }
                else if (pieces[i] == 'p')
                {
                    // Generate white pawn captures.
                    if (COL(i) != 0 && std::isupper(pieces[i - 7])) moves.push_back(Move(i, i - 7, pieces[i - 7]));
                    if (COL(i) != 0 && std::isupper(pieces[i - 9])) moves.push_back(Move(i, i - 9, pieces[i - 9]));
                    // Generate white pawn push and long push.
                    if (pieces[i - 8] == 0x00)
                    {
                        moves.push_back(Move(i, i - 8));
                        if (pieces[i - 16] == 0x00 && i >= 48) moves.push_back(Move(i, i - 16));
                    }
                }
                else
                {
                    int piece = 0;
                    if (std::toupper(pieces[i]) == 'N') piece = 1;
                    else if (std::toupper(pieces[i]) == 'B') piece = 2;
                    else if (std::toupper(pieces[i]) == 'R') piece = 3;
                    else if (std::toupper(pieces[i]) == 'Q') piece = 4;
                    else if (std::toupper(pieces[i]) == 'K') piece = 5;

                    bool slide[6] = {
	                    false, false, true, true, true, false
                    };

                    int offset[6][8] = {
	                    { 0, 0, 0, 0, 0, 0, 0, 0 },
	                    { -21, -19, -12, -8, 8, 12, 19, 21 },
	                    { -11, -9, 9, 11, 0, 0, 0, 0 },
	                    { -10, -1, 1, 10, 0, 0, 0, 0 },
	                    { -11, -10, -9, -1, 1, 9, 10, 11 },
	                    { -11, -10, -9, -1, 1, 9, 10, 11 }
                    };

                    int16_t offsets[6] = {
	                    0, 8, 4, 4, 8, 8
                    };

                    for (uint16_t j = 0; j < offsets[piece]; ++j)
                    {
                        for (uint16_t target = i;;)
                        {
                            target = mailbox[mailbox64[target] + offset[piece][j]];
                            if (target == UINT16_MAX) break; /* Check if target is out of board. */
                            if (pieces[target] != 0x00)
                            {
                                if (std::isupper(pieces[target]) && !whiteSideToMove || std::islower(pieces[target]) && whiteSideToMove)
                                {
                                    moves.push_back(Move(i, target, pieces[target]));
                                }
                                break;
                            }
                            moves.push_back(Move(i, target, pieces[target]));
                            if (!slide[piece]) break;
                        }
                    }
                }
            }
            /*if (pieces[i] == 'P')
            {
                if (pieces[i + 10] == 0x00) moves.push_back(Move(i, i + 10));
                else if (pieces[i + 20] == 0x00 && i > 29 && i < 39) moves.push_back(Move(i, i + 20));
                else if (pieces[i + 11] != 0x00 && !std::isupper(pieces[i + 11])) moves.push_back(Move(i, i + 11));
                else if (pieces[i + 9] != 0x00 && !std::isupper(pieces[i + 9])) moves.push_back(Move(i, i + 9));
            }
            else if (pieces[i] == 'p')
            {
                if (pieces[i - 10] == 0x00) moves.push_back(Move(i, i - 10));
                else if (pieces[i - 20] == 0x00 && i > 79 && i < 90) moves.push_back(Move(i, i - 20));
                else if (pieces[i - 11] != 0x00 && std::isupper(pieces[i - 11])) moves.push_back(Move(i, i - 11));
                else if (pieces[i - 9] != 0x00 && std::isupper(pieces[i - 9])) moves.push_back(Move(i, i - 9));
            }
            else if (pieces[i] == 'N' || pieces[i] == 'n')
            {
                // Knight offsets(10x12): { -21, -19, -12, -8, 8, 12, 19, 21 }
                if (pieces[i - 21] == 0x00) moves.push_back(Move(i, i - 21));
                else if (pieces[i - 19] == 0x00) moves.push_back(Move(i, i - 19));
                else if (pieces[i - 12] == 0x00) moves.push_back(Move(i, i - 12));
                else if (pieces[i - 8] == 0x00) moves.push_back(Move(i, i - 8));
                else if (pieces[i + 8] == 0x00) moves.push_back(Move(i, i + 8));
                else if (pieces[i + 12] == 0x00) moves.push_back(Move(i, i + 12));
                else if (pieces[i + 19] == 0x00) moves.push_back(Move(i, i + 19));
                else if (pieces[i + 21] == 0x00) moves.push_back(Move(i, i + 21));
            }
            else if (pieces[i] == 'K' || pieces[i] == 'k')
            {
                // King offsets(10x12): { -11, -10, -9, -1, 1,  9, 10, 11 }
                if (pieces[i - 11] == 0x00) moves.push_back(Move(i, i - 11));
                else if (pieces[i - 10] == 0x00) moves.push_back(Move(i, i - 10));
                else if (pieces[i - 9] == 0x00) moves.push_back(Move(i, i - 9));
                else if (pieces[i - 1] == 0x00) moves.push_back(Move(i, i - 1));
                else if (pieces[i + 1] == 0x00) moves.push_back(Move(i, i + 1));
                else if (pieces[i + 9] == 0x00) moves.push_back(Move(i, i + 9));
                else if (pieces[i + 10] == 0x00) moves.push_back(Move(i, i + 10));
                else if (pieces[i + 11] == 0x00) moves.push_back(Move(i, i + 11));
            }
            else if (pieces[i] == 'B' || pieces[i] == 'b')
            {
                // Bishop offsets(10x12): { -11,  -9,  9, 11, 0,  0,  0,  0 }
            }
            else if (pieces[i] == 'R' || pieces[i] == 'r')
            {
                // Rook offsets(10x12): { -10,  -1,  1, 10, 0,  0,  0,  0 }
            }
            else if (pieces[i] == 'Q' || pieces[i] == 'q')
            {
                // Queen offsets(10x12): { -11, -10, -9, -1, 1,  9, 10, 11 }
            }*/
        }

        // Generate castle moves.
        if (whiteSideToMove)
        {

        }

        // Generate en passant moves.


        return moves;
    }

public:
    Piece pieces[64];
    bool whiteSideToMove = true;

    /* the en passant square. if white moves e2e4, the en passant
    square is set to e3, because that's where a pawn would move
	in an en passant capture */
    uint16_t enPassantSquare = UINT16_MAX;

};

StandardBoard board;

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
    std::transform(commandString.begin(), commandString.end(), commandString.begin(), ::toupper);
    // std::cout << "COMMAND: " << commandString << std::endl;

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
        }
        else if (commandComponents[0] == "PRINT") { board.Print(); }
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

                Move move = Move(originIndex, targetIndex, board.pieces[targetIndex]);
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
                std::cout << "Perft(" << depth << "): " << board.Perft<true>(depth) << std::endl;
                /*std::vector<Move> moves = board.GetLegalMoves();
                for (Move move : moves)
                {
                    std::string UCI = "0000";
                    UCI[0] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.OriginSquare)];
                    UCI[1] = std::string("123456789")[ROW(move.OriginSquare)];
                    UCI[2] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.TargetSquare)];
                    UCI[3] = std::string("123456789")[ROW(move.TargetSquare)];
                    board.MakeMove(move);
                    std::cout << UCI << ": " << board.Perft(depth) << std::endl;
                    board.UndoMove(move);
                }*/
            }
            else
            {
                std::cout << "The syntax of the command is incorrect." << std::endl;
            }
        }
        else if (commandComponents[0] == "LEGALMOVES")
        {
            std::vector<Move> moves = board.GetLegalMoves();
            for (Move move : moves)
            {
                std::string UCI = "0000";
                UCI[0] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.OriginSquare)];
                UCI[1] = std::string("123456789")[ROW(move.OriginSquare)];
                UCI[2] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.TargetSquare)];
                UCI[3] = std::string("123456789")[ROW(move.TargetSquare)];

                std::cout << UCI << std::endl;
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

/* Application */
int main(int argc, char* argv[])
{
    board = StandardBoard(); 

    //std::vector<Move> moves = board.GetLegalMoves();
    //std::cout << "Perft(1) -> " << moves.size() << std::endl;

    std::cout << "Caissa 1.0.0" << std::endl;

    CommandLoop();
}