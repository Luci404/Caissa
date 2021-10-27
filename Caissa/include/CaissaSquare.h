#pragma once

#include "CaissaPiece.h"

#include <optional>

class CaissaSquare
{
    public:
        CaissaSquare();

        bool Occupied() const;

        void SetPiece(CaissaPiece piece);
    
    private:
        std::optional<CaissaPiece> m_Piece;
};