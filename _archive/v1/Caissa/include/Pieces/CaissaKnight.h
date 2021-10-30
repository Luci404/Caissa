#pragma once

#include "CaissaPiece.h"

class CaissaKnight : public CaissaPiece
{
    public:
        CaissaKnight(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'n'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaKnight::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file) override;
};