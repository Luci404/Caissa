#pragma once

#include "CaissaPiece.h"

class CaissaBishop : public CaissaPiece
{
    public:
        CaissaBishop(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'b'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaBishop::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file) override;
};