#include "CaissaSquare.h"
#include "CaissaPiece.h"

CaissaSquare::CaissaSquare()
    : m_Piece(std::optional<CaissaPiece>())
{
}

bool CaissaSquare::Occupied() const
{
    return m_Piece.has_value();
}

void CaissaSquare::SetPiece(CaissaPiece piece)
{
    m_Piece = piece;
}