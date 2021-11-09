#pragma once

#include <inttypes.h>
#include <vector>
#include <string>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <ctype.h>

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
        // Find stuff.
    };*/

    struct Move
    {
    public:
        Move(uint16_t originIndex, uint16_t targetIndex, Piece capturePiece = 0x00, MoveType moveType = MoveType::NORMAL)
            : OriginIndex(originIndex), TargetIndex(targetIndex), CapturePiece(capturePiece), Type(moveType)
        {
        }

    public:
        const uint16_t OriginIndex;
        const uint16_t TargetIndex;
        const Piece CapturePiece;
        const MoveType Type;
    };

    class Position
    {
    public:
        virtual std::string ToString() const = 0;
        virtual bool IsCheck() const = 0;
        virtual void MakeMove(Move move) = 0;
        virtual void UndoMove(Move move) = 0;
        virtual std::vector<Move> GetLegalMoves() const = 0;
    };

    class StandardBoard : public Position
    {
    public:
        StandardBoard(std::string fen)
        {
            // Initialize piece array.
            for (uint16_t i = 0; i < 64; ++i)
            {
                pieces[i] = 0x00;
            }

            // Parse FEN.
            std::reverse(fen.begin(), fen.end());
            std::istringstream ss(fen);
            uint16_t square = 0;
            uint8_t token;

            while ((ss >> token) && !std::isspace(token))
            {
                if (std::isdigit(token))
                {
                    square += (token - '0') * 1;
                }
                else if (token == '/')
                {
                    continue;
                }
                else if (token != std::string::npos)
                {
                    pieces[square] = token;
                    ++square;
                }
            }
        }

        StandardBoard()
        {
        }

        bool IsAttacking(uint16_t squareIndex, bool side) const
        {
            std::vector<Move> moves = GetLegalMoves();
            for (Move move : moves)
            {
                if (move.TargetIndex == squareIndex && (bool)std::isupper(move.OriginIndex) == side)
                {
                    return true;
                }
            }

            return false;
        }

        /*
        Returns a Forsyth–Edwards Notation (FEN) string representing the current board possition.
        */
        std::string Fen() const
        {
            int emptyCount;
            std::ostringstream ss;

            for (int16_t rank = 7; rank >= 0; rank--)
            {
                for (uint16_t file = 0; file <= 7; file++)
                {
                    for (emptyCount = 0; file <= 7 && pieces[rank * 8 + file] == 0x00; ++file)
                    {
                        ++emptyCount;
                    }

                    if (emptyCount)
                    {
                        ss << emptyCount;
                    }

                    if (file <= 7)
                    {
                        ss << pieces[rank * 8 + file];
                    }
                }

                if (rank > 0)
                {
                    ss << '/';
                }
            }

            return ss.str();
        }
        /*
        Perft is a utility for verifying move generation.
        All the leaf nodes up to the given depth are generated and counted, and the sum is returned.
         - https://www.chessprogramming.org/Perft
         - https://www.chessprogramming.org/Perft_Results
        */
        uint64_t Perft(int depth, bool root = true)
        {
            uint64_t nodes = 0;
            uint64_t count = 0;

            const bool leaf = (depth == 2);

            for (const Move &move : GetLegalMoves())
            {
                if (root && depth <= 1)
                {
                    count = 1;
                    nodes++;
                }
                else
                {
                    MakeMove(move);
                    count = leaf ? GetLegalMoves().size() : Perft(depth - 1, false);
                    nodes += count;
                    UndoMove(move);
                }

                if (root)
                {
                    std::string UCI = "0000";
                    UCI[0] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.OriginIndex)];
                    UCI[1] = std::string("123456789")[ROW(move.OriginIndex)];
                    UCI[2] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.TargetIndex)];
                    UCI[3] = std::string("123456789")[ROW(move.TargetIndex)];
                    std::cout << UCI << ": " << count << std::endl;
                }
            }

            return nodes;
        }

        /*
        Returns an ASCII representation of the position.
        */
        std::string ToString() const override
        {
            std::stringstream ss;

            ss << "+-- a b c d e f g h --+" << std::endl;
            ss << "|                     |" << std::endl;
            for (int16_t rank = 7; rank >= 0; rank--)
            {
                ss << rank + 1 << "   ";
                for (uint16_t file = 0; file < 8; file++)
                {
                    if (rank * 8 + file == enPassantIndex)
                    {
                        ss << "ep";
                    }
                    
                    // ss << rank * 8 + file;
                
                    ss << (char)(pieces[rank * 8 + file] == 0x00 ? '-' : pieces[rank * 8 + file]) << " ";
                }
                ss << "  " << rank + 1 << std::endl;
            }
            ss << "|                     |" << std::endl;
            ss << "+-- a b c d e f g h --+" << std::endl;

            return ss.str();
        }

        virtual bool IsCheck() const override
        {
            for (uint16_t i = 0; i < 64; ++i)
            {
                if (std::toupper(pieces[i]) == 'K' && (bool)std::isupper(pieces[i]) == whiteSideToMove)
                {
                    return IsAttacking(i, !whiteSideToMove);
                }
            }

            return true; /* This should never be reached. */
        }

        virtual void MakeMove(Move move) override
        {
            whiteSideToMove = (bool)std::islower(pieces[move.OriginIndex]);
            pieces[move.TargetIndex] = pieces[move.OriginIndex];
            pieces[move.OriginIndex] = 0x00;
            
            // Double pawn push
            if (std::toupper(pieces[move.TargetIndex]) == 'P' && abs(ROW(move.OriginIndex) - ROW(move.TargetIndex)) == 2)
            {
                oldEnPassantIndex = enPassantIndex;
                enPassantIndex = whiteSideToMove ? move.TargetIndex + 8 : move.TargetIndex - 8;
            }
            else
            {
                oldEnPassantIndex = enPassantIndex;
                enPassantIndex = UINT32_MAX;
            }

            // En passant
            if (move.Type == MoveType::EN_PASSANT)
            {
                if (std::isupper(move.CapturePiece))
                {
                    // White piece captured
                    pieces[move.TargetIndex + 8] = 0x00;
                }
                else
                {
                    // Black piece captured
                    pieces[move.TargetIndex - 8] = 0x00;
                }
            }
            
        }

        virtual void UndoMove(Move move) override
        {

            // En passant
            if (move.Type == MoveType::EN_PASSANT)
            {
                if (std::isupper(move.CapturePiece))
                {
                    // White piece captured
                    pieces[move.TargetIndex + 8] = move.CapturePiece;
                }
                else
                {
                    // Black piece captured
                    pieces[move.TargetIndex - 8] = move.CapturePiece;
                }
            }

            // Double pawn push
            if (std::toupper(pieces[move.TargetIndex]) == 'P' && abs(ROW(move.OriginIndex) - ROW(move.TargetIndex)) == 2)
            {
                enPassantIndex = oldEnPassantIndex;
                oldEnPassantIndex = UINT16_MAX;
            }
            else
            {
                enPassantIndex = oldEnPassantIndex;
                oldEnPassantIndex = UINT32_MAX;
            }

            pieces[move.OriginIndex] = pieces[move.TargetIndex];
            pieces[move.TargetIndex] = move.Type == MoveType::EN_PASSANT ? 0x00 : move.CapturePiece;
            whiteSideToMove = (bool)std::isupper(pieces[move.OriginIndex]);
        }

        std::vector<Move> GetLegalMoves() const override
        {
            std::vector<Move> moves;

            for (uint16_t i = 0; i < 64; ++i)
            {
                if ((bool)std::isupper(pieces[i]) == whiteSideToMove)
                {
                    if (pieces[i] == 'P')
                    {
                        // Generate white pawn captures.
                        if (COL(i) != 0 && std::islower(pieces[i + 7]))
                            moves.push_back(Move(i, i + 7, pieces[i + 7]));
                        if (COL(i) != 7 && std::islower(pieces[i + 9]))
                            moves.push_back(Move(i, i + 9, pieces[i + 9]));

                        // Generate white pawn push and long push.
                        if (pieces[i + 8] == 0x00)
                        {
                            moves.push_back(Move(i, i + 8, pieces[i + 8]));
                            if (pieces[i + 16] == 0x00 && i <= 15)
                                moves.push_back(Move(i, i + 16, pieces[i + 16]));
                        }
                    }
                    else if (pieces[i] == 'p')
                    {

                        // Generate white pawn captures.
                        if (COL(i) != 0 && std::isupper(pieces[i - 7]))
                            moves.push_back(Move(i, i - 7, pieces[i - 7]));
                        if (COL(i) != 0 && std::isupper(pieces[i - 9]))
                            moves.push_back(Move(i, i - 9, pieces[i - 9]));

                        // Generate white pawn push and long push.
                        if (pieces[i - 8] == 0x00)
                        {
                            moves.push_back(Move(i, i - 8));
                            if (pieces[i - 16] == 0x00 && i >= 48)
                                moves.push_back(Move(i, i - 16));
                        }
                    }
                    else
                    {
                        int piece = 0;
                        if (std::toupper(pieces[i]) == 'N')
                            piece = 1;
                        else if (std::toupper(pieces[i]) == 'B')
                            piece = 2;
                        else if (std::toupper(pieces[i]) == 'R')
                            piece = 3;
                        else if (std::toupper(pieces[i]) == 'Q')
                            piece = 4;
                        else if (std::toupper(pieces[i]) == 'K')
                            piece = 5;

                        bool slide[6] = {
                            false, false, true, true, true, false};

                        int offset[6][8] = {
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {-21, -19, -12, -8, 8, 12, 19, 21},
                            {-11, -9, 9, 11, 0, 0, 0, 0},
                            {-10, -1, 1, 10, 0, 0, 0, 0},
                            {-11, -10, -9, -1, 1, 9, 10, 11},
                            {-11, -10, -9, -1, 1, 9, 10, 11}};

                        int16_t offsets[6] = {
                            0, 8, 4, 4, 8, 8};

                        for (uint16_t j = 0; j < offsets[piece]; ++j)
                        {
                            for (uint16_t target = i;;)
                            {
                                target = mailbox[mailbox64[target] + offset[piece][j]];
                                if (target == UINT16_MAX)
                                    break; /* Check if target is out of board. */
                                if (pieces[target] != 0x00)
                                {
                                    if (std::isupper(pieces[target]) && !whiteSideToMove || std::islower(pieces[target]) && whiteSideToMove)
                                    {
                                        moves.push_back(Move(i, target, pieces[target]));
                                    }
                                    break;
                                }
                                moves.push_back(Move(i, target, pieces[target]));
                                if (!slide[piece])
                                    break;
                            }
                        }
                    }
                }
            }

            // TODO: Generate castle moves.
            
            // Generate en passant moves.
            if (enPassantIndex != UINT32_MAX)
            {
                if (whiteSideToMove)
                {
                    if (pieces[enPassantIndex - 7] == 'P') moves.push_back(Move(enPassantIndex - 7, enPassantIndex, pieces[enPassantIndex - 8], MoveType::EN_PASSANT));
                    else if (pieces[enPassantIndex - 9] == 'P') moves.push_back(Move(enPassantIndex - 9, enPassantIndex, pieces[enPassantIndex - 8], MoveType::EN_PASSANT));
                }
                else
                {
                    if (pieces[enPassantIndex + 7] == 'p') moves.push_back(Move(enPassantIndex + 7, enPassantIndex, pieces[enPassantIndex + 8], MoveType::EN_PASSANT));
                    else if (pieces[enPassantIndex + 9] == 'p') moves.push_back(Move(enPassantIndex + 9, enPassantIndex, pieces[enPassantIndex + 8], MoveType::EN_PASSANT));
                }
            }

            return moves;
        }

    public:
        Piece pieces[64];
        uint32_t enPassantIndex = UINT32_MAX;
        uint32_t oldEnPassantIndex = UINT32_MAX; // Old is ONLY for undo move.
        bool whiteSideToMove = true;
    };
}