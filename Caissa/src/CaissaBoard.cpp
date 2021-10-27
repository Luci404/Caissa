#include "CaissaBoard.h"
#include "CaissaSquare.h"

CaissaBoard::CaissaBoard(uint16_t rankCount, uint16_t fileCount)
    : m_RankCount(rankCount), m_FileCount(fileCount)
{
    for(std::vector<CaissaSquare*> rank : m_Squares)
    {
        rank = std::vector<CaissaSquare*>(8);
    }
}