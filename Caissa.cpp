#include <inttypes.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <bits/stdc++.h>

#include "Caissa.h"

namespace Caissa
{
    StandardBoard::StandardBoard(std::string fen)
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

    StandardBoard::StandardBoard()
    {
        // Init
        for (int i = 0; i < 64; i++)
        {
            // White
            if (i == 0 || i == 7)
                pieces[i] = 'R';
            else if (i == 1 || i == 6)
                pieces[i] = 'N';
            else if (i == 2 || i == 5)
                pieces[i] = 'B';
            else if (i == 3)
                pieces[i] = 'Q';
            else if (i == 4)
                pieces[i] = 'K';
            else if (i > 7 && i < 16)
                pieces[i] = 'P';
            // Black
            else if (i == 7 * 8 + 0 || i == 7 * 8 + 7)
                pieces[i] = 'r';
            else if (i == 7 * 8 + 1 || i == 7 * 8 + 6)
                pieces[i] = 'n';
            else if (i == 7 * 8 + 2 || i == 7 * 8 + 5)
                pieces[i] = 'b';
            else if (i == 7 * 8 + 3)
                pieces[i] = 'q';
            else if (i == 7 * 8 + 4)
                pieces[i] = 'k';
            else if (i > 47 && i < 56)
                pieces[i] = 'p';
            // Empty
            else
                pieces[i] = 0x00;
        }
    }

    /*
    Returns true if the specified square is beeing attack by the specified side.
    */
    bool StandardBoard::IsAttacking(uint16_t squareIndex, bool side) const
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

    std::string StandardBoard::Fen() const
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

    uint64_t StandardBoard::Perft(int depth, bool root)
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
                UCI[0] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.OriginSquare)];
                UCI[1] = std::string("123456789")[ROW(move.OriginSquare)];
                UCI[2] = std::string("abcdefghijklmnopqrstuvwxyz")[COL(move.TargetSquare)];
                UCI[3] = std::string("123456789")[ROW(move.TargetSquare)];
                std::cout << UCI << ": " << count << std::endl;
            }
        }

        return nodes;
    }

    void StandardBoard::Print() const
    {
        std::cout << "+-- a b c d e f g h --+" << std::endl;
        std::cout << "|                     |" << std::endl;
        for (int16_t rank = 7; rank >= 0; rank--)
        {
            std::cout << rank + 1 << "   ";
            for (uint16_t file = 0; file < 8; file++)
            {
                std::cout << (char)(pieces[rank * 8 + file] == 0x00 ? '-' : pieces[rank * 8 + file]) << " ";
            }
            std::cout << "  " << rank + 1 << std::endl;
        }
        std::cout << "|                     |" << std::endl;
        std::cout << "+-- a b c d e f g h --+" << std::endl;
    }

    bool StandardBoard::IsCheck() const
    {
        for (uint16_t i = 0; i < 64; ++i)
        {
            if (std::toupper(pieces[i]) == 'K' && std::isupper(pieces[i]) == whiteSideToMove)
            {
                return IsAttacking(i, !whiteSideToMove);
            }
        }

        return true; /* This should never be reached. */
    }

    void StandardBoard::MakeMove(Move move)
    {
        whiteSideToMove = std::islower(pieces[move.OriginSquare]);
        pieces[move.TargetSquare] = pieces[move.OriginSquare];
        pieces[move.OriginSquare] = 0x00;
    }

    void StandardBoard::UndoMove(Move move)
    {
        pieces[move.OriginSquare] = pieces[move.TargetSquare];
        pieces[move.TargetSquare] = move.CapturePiece;
        whiteSideToMove = std::isupper(pieces[move.OriginSquare]);
    }

    std::vector<Move> StandardBoard::GetLegalMoves() const
    {
        std::vector<Move> moves;

        for (uint16_t i = 0; i < 64; ++i)
        {
            if (std::isupper(pieces[i]) == whiteSideToMove)
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
        // TODO: Generate en passant moves.

        return moves;
    }

}