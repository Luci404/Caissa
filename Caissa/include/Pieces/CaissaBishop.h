#pragma once

#include "CaissaPiece.h"

class CaissaBishop : public CaissaPiece
{
    public:
        CaissaBishop(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'b'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaBishop::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves() override;
};