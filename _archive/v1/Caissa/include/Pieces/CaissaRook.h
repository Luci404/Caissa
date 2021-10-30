#pragma once

#include "CaissaPiece.h"

class CaissaRook : public CaissaPiece
{
    public:
        CaissaRook(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'r'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaRook::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file) override;
};