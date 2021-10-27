#include "CaissaBoard.h"
#include "CaissaSquare.h"
#include "CaissaPiece.h"

CaissaBoard::CaissaBoard(uint16_t rankCount, uint16_t fileCount)
    : m_RankCount(rankCount), m_FileCount(fileCount)
{
    for (uint16_t rank = 0; rank < m_RankCount; rank++)
    {
        m_Squares.push_back(std::vector<CaissaSquare>(m_FileCount));

        for (uint16_t file = 0; file < m_FileCount; file++)
        {
            m_Squares[rank][file] = CaissaSquare();
        }
    }
}

bool CaissaBoard::AddPiece(CaissaPiece piece, uint16_t rank, uint16_t file)
{
    CaissaSquare* targetSquare = GetSquare(rank, file);
    if (targetSquare == nullptr) return false;

    if (targetSquare->Occupied()) return false;
    
    targetSquare->SetPiece(piece);

    return true;
}

bool CaissaBoard::Move(uint16_t originRank, uint16_t originFile, uint16_t targetRank, uint16_t targetFile)
{
    CaissaSquare* originSquare = GetSquare(originRank, originFile);
    CaissaSquare* targetSquare = GetSquare(targetRank, targetFile);
    if (originSquare == nullptr || targetSquare == nullptr) return false;

    // TODO: Move pieces.

    return true;
}

CaissaSquare* CaissaBoard::GetSquare(uint16_t rank, uint16_t file)
{
    if (rank >= m_Squares.size() || file >= m_Squares[rank].size()) return nullptr;

    return &m_Squares[rank][file];
}