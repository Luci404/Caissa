#include <inttypes.h>
#include <iostream>
#include <vector>

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

struct Move
{
public:
    Move(uint16_t originSquare, uint16_t targetSquare)
        : OriginSquare(originSquare), TargetSquare(targetSquare)
    {
    }

public:
    const uint16_t OriginSquare;
    const uint16_t TargetSquare;
};

class Board 
{
public:
    virtual void Print() const = 0;    
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
            else if (i > 6 * 8 - 1 && i < 8 * 8 + 0) pieces[i] = 'p';
            //else if (i > 6 * 8 - 1 && i < 8 * 8 + 0) pieces[i] = 'p';
            else pieces[i] = 0x00; 
        }
    }

    virtual void Print() const override
    {   
        std::cout << "    a b c d e f g h     " << std::endl << std::endl;
        for (int16_t rank = 7; rank >= 0; rank--)
        {
            std::cout << rank << "   ";
            for (uint16_t file = 0; file < 8; file++) 
            {
                std::cout << (char)(pieces[rank * 8 + file] == 0x00 ? '-' : pieces[rank * 8 + file]) << " ";
            }
            std::cout << "  " << rank << std::endl;
        }
        std::cout << std::endl << "    a b c d e f g h    " << std::endl;
    }

    uint64_t Perft(int depth)
    {
        uint64_t nodes = 0;

        if (depth == 1)  { return GetLegalMoves().size(); }

        return nodes;
    }

    virtual void MakeMove(Move move) override
    {
        pieces[move.TargetSquare] = pieces[move.OriginSquare];
        pieces[move.OriginSquare] = 0x00;
    }

    virtual void UndoMove(Move move) override
    {
        // THIS DOES NOT WORK...
        pieces[move.OriginSquare] = pieces[move.TargetSquare];
        pieces[move.TargetSquare] = 0x00;
    }

    std::vector<Move> GetLegalMoves() const override
    {
        std::vector<Move> moves;

        for (uint16_t i = 0; i < 64; ++i)
        {

            if (pieces[i] == 'P')
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
            }
        }

        return moves;
    }

public:
    Piece pieces[64];
};

/* Application */
int main(int argc, char* argv[])
{
    StandardBoard board = StandardBoard(); 
    board.Print();

    std::cout << "Perft(1) -> " << board.Perft(1) << std::endl;
    
    /*for (Move move : moves)
    {
        board.MakeMove(move);
        std::cout << std::endl << "Move: " << move.OriginSquare << " -> " << move.TargetSquare << std::endl;
        board.Print();
        board.MakeMove(Move(move.TargetSquare, move.OriginSquare));
    }*/
}