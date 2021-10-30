#include <inttypes.h>
#include <iostream>
#include <vector>

/* TODO: ♙♘♗♖♕♔·♟♞♝♜♛♚ */

/* Board Representation */
typedef uint8_t Piece;

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
                if (pieces[i + 8] == 0x00) moves.push_back(Move(i, i + 8));
                if (pieces[i + 8 * 2] == 0x00 && i > 7 && i < 16) moves.push_back(Move(i, i + 8 * 2));
            }
            else if (pieces[i] == 'N')
            {
                if (pieces[i + 15] == 0x00) moves.push_back(Move(i, i + 15));
                if (pieces[i + 17] == 0x00) moves.push_back(Move(i, i + 17));
                if (pieces[i - 15] == 0x00) moves.push_back(Move(i, i - 15));
                if (pieces[i - 17] == 0x00) moves.push_back(Move(i, i - 17));
                if (pieces[i + 10] == 0x00) moves.push_back(Move(i, i + 10));
                if (pieces[i + 6] == 0x00) moves.push_back(Move(i, i + 6));
                if (pieces[i - 10] == 0x00) moves.push_back(Move(i, i - 10));
                if (pieces[i - 6] == 0x00) moves.push_back(Move(i, i - 6));
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