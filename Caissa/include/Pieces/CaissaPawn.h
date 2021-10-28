#pragma once

#include "CaissaPiece.h"

class CaissaPawn : public CaissaPiece
{
    public:
        CaissaPawn(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'p'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaPawn::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves() override;
};