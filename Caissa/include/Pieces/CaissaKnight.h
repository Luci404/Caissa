#pragma once

#include "CaissaPiece.h"

class CaissaKnight : public CaissaPiece
{
    public:
        CaissaKnight(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'n'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaKnight::PieceIdentifier(); }
};