#pragma once

#include <stdint.h>
#include <vector>

class CaissaSquare;

class CaissaBoard
{
    public:
        CaissaBoard(uint16_t rankCount, uint16_t fileCount);

        uint16_t GetRankCount() const { return m_RankCount; }
        uint16_t GetFileCount() const { return m_FileCount; }
        
    private:
        uint16_t m_RankCount;
        uint16_t m_FileCount;

        std::vector<std::vector<CaissaSquare*>> m_Squares;
};