#pragma once

#include "CaissaPiece.h"

class CaissaQueen : public CaissaPiece
{
    public:
        CaissaQueen(CaissaTeamIdentifier teamIdentifier) : CaissaPiece(teamIdentifier) {};

        static CaissaPieceIdentifier PieceIdentifier() { return 'q'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaQueen::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file) override;
};