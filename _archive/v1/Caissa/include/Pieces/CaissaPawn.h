#pragma once

#include "CaissaPiece.h"

class CaissaPawn : public CaissaPiece
{
    public:
        CaissaPawn(CaissaTeamIdentifier teamIdentifier);

        static CaissaPieceIdentifier PieceIdentifier() { return 'p'; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaPawn::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file) override;
    private:
        bool m_HasMoved;
};