#pragma once

#include "CaissaPiece.h"

class CaissaRook : public CaissaPiece
{
    public:
        CaissaRook(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'r'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaRook::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves() override;
};