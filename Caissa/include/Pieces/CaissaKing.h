#pragma once

#include "CaissaPiece.h"

class CaissaKing : public CaissaPiece
{
    public:
        CaissaKing(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'k'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaKing::PieceIdentifier(); }
};