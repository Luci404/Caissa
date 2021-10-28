#pragma once

#include "Caissa.h"

#include <stdint.h>
#include <vector>
#include <memory>

class CaissaSquare;
class CaissaPiece;

class CaissaBoard
{
    public:
        CaissaBoard(uint16_t rankCount, uint16_t fileCount);

        uint16_t GetRankCount() const { return m_RankCount; }
        uint16_t GetFileCount() const { return m_FileCount; }

        bool AddPiece(std::shared_ptr<CaissaPiece> piece, uint16_t rank, uint16_t file);
        bool Move(uint16_t originRank, uint16_t originFile, uint16_t targetRank, uint16_t targetFile);

        CaissaSquare* GetSquare(uint16_t rank, uint16_t file);

        // Move generation
        void GeneratePseudolegalMoves();

    private:
        uint16_t m_RankCount;
        uint16_t m_FileCount;

        std::vector<std::vector<CaissaSquare>> m_Squares;

        // Move generation
        std::vector<CaissaMove> m_PseudolegalMoves;
};