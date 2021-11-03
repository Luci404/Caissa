#pragma once

#include <inttypes.h>
#include <vector>
#include <string>

namespace Caissa
{
#define ROW(x) (x >> 3)
#define COL(x) (x & 7)

    static uint16_t mailbox[120] = {
        UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX,
        UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX,
        UINT16_MAX, 0, 1, 2, 3, 4, 5, 6, 7, UINT16_MAX,
        UINT16_MAX, 8, 9, 10, 11, 12, 13, 14, 15, UINT16_MAX,
        UINT16_MAX, 16, 17, 18, 19, 20, 21, 22, 23, UINT16_MAX,
        UINT16_MAX, 24, 25, 26, 27, 28, 29, 30, 31, UINT16_MAX,
        UINT16_MAX, 32, 33, 34, 35, 36, 37, 38, 39, UINT16_MAX,
        UINT16_MAX, 40, 41, 42, 43, 44, 45, 46, 47, UINT16_MAX,
        UINT16_MAX, 48, 49, 50, 51, 52, 53, 54, 55, UINT16_MAX,
        UINT16_MAX, 56, 57, 58, 59, 60, 61, 62, 63, UINT16_MAX,
        UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX,
        UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX};

    static uint16_t mailbox64[64] = {
        21, 22, 23, 24, 25, 26, 27, 28,
        31, 32, 33, 34, 35, 36, 37, 38,
        41, 42, 43, 44, 45, 46, 47, 48,
        51, 52, 53, 54, 55, 56, 57, 58,
        61, 62, 63, 64, 65, 66, 67, 68,
        71, 72, 73, 74, 75, 76, 77, 78,
        81, 82, 83, 84, 85, 86, 87, 88,
        91, 92, 93, 94, 95, 96, 97, 98};

    typedef uint8_t Piece;
    
    enum MoveType : uint8_t
    {
        NORMAL,
        PROMOTION,
        EN_PASSANT,
        CASTLING
    };

    /*enum GenType {
        CAPTURES,
        QUIETS,
        QUIET_CHECKS,
        EVASIONS,
        NON_EVASIONS,
        LEGAL
    };
    
    template<GenType T>
    struct MoveList
    {
    };*/

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


    class Position
    {
    public:
        virtual void Print() const = 0;
        virtual bool IsCheck() const = 0;
        virtual void MakeMove(Move move) = 0;
        virtual void UndoMove(Move move) = 0;
        virtual std::vector<Move> GetLegalMoves() const = 0;
    };

    class StandardBoard : public Position
    {
    public:
        StandardBoard(std::string fen);
        StandardBoard();

        bool IsAttacking(uint16_t squareIndex, bool side) const;

        std::string Fen() const;

        /*
        Counts all the leaf nodes of a certain depth.
         - https://www.chessprogramming.org/Perft
         - https://www.chessprogramming.org/Perft_Results
        */
        uint64_t Perft(int depth, bool root = true);

        virtual void Print() const override;
        virtual bool IsCheck() const override;
        virtual void MakeMove(Move move) override;
        virtual void UndoMove(Move move) override;
        std::vector<Move> GetLegalMoves() const override;

    public:
        Piece pieces[64];
        bool whiteSideToMove = true;

        /* the en passant square. if white moves e2e4, the en passant
        square is set to e3, because that's where a pawn would move
    	in an en passant capture */
        uint16_t enPassantSquare = UINT16_MAX;
    };
}