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
        CaissaBoard(std::shared_ptr<CaissaConfiguration> configuration);

        uint16_t GetRankCount() const { return m_Configuration->RankCount; }
        uint16_t GetFileCount() const { return m_Configuration->FileCount; }

        bool AddPiece(std::shared_ptr<CaissaPiece> piece, uint16_t rank, uint16_t file);
        bool Move(uint16_t originRank, uint16_t originFile, uint16_t targetRank, uint16_t targetFile);

        CaissaSquare* GetSquare(uint16_t rank, uint16_t file);

        // Move generation
        void GeneratePseudolegalMoves();

    private:
        std::shared_ptr<CaissaConfiguration> m_Configuration;

        std::vector<std::vector<CaissaSquare>> m_Squares;

        // Move generation
        std::vector<CaissaMove> m_PseudolegalMoves;
};