#pragma once

#include "CaissaPiece.h"

class CaissaQueen : public CaissaPiece
{
    public:
        CaissaQueen(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'q'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaQueen::PieceIdentifier(); }
};