#pragma once

#include "Caissa.h"

class CaissaPiece
{
    public:
        CaissaPiece() = default;
        CaissaPiece(CaissaTeamIdentifier teamIdentifier);

        static CaissaPieceIdentifier PieceIdentifier() { return CAISSA_PIECEID_INVALID; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaPiece::PieceIdentifier(); }

        virtual std::vector<CaissaMove> GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file);

        CaissaTeamIdentifier GetTeamIdentifier() const { return m_TeamIdentifier; }
    
    private:
        CaissaTeamIdentifier m_TeamIdentifier;
};