#pragma once

#include "CaissaPiece.h"

class CaissaKing : public CaissaPiece
{
    public:
        CaissaKing(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'k'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaKing::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file) override;
};