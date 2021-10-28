#include "CaissaSquare.h"
#include "CaissaPiece.h"

CaissaSquare::CaissaSquare()
    : m_Piece(nullptr)
{
}

bool CaissaSquare::Occupied() const
{
    return m_Piece != nullptr;
}

std::shared_ptr<CaissaPiece> CaissaSquare::GetPiece()
{
    return m_Piece;
}

void CaissaSquare::SetPiece(std::shared_ptr<CaissaPiece> piece)
{
    m_Piece = piece;
}