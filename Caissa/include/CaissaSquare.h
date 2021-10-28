#pragma once

#include "CaissaPiece.h"

#include <memory>

class CaissaSquare
{
    public:
        CaissaSquare();

        bool Occupied() const;

        std::shared_ptr<CaissaPiece> GetPiece();
        void SetPiece(std::shared_ptr<CaissaPiece> piece);
    
    private:
        std::shared_ptr<CaissaPiece> m_Piece;
};