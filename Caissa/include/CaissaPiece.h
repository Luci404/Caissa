#pragma once

#include "Caissa.h"

class CaissaPiece
{
    public:
        CaissaPiece() = default;
        CaissaPiece(CaissaTeamIdentifier teamIdentifier);

        static CaissaPieceIdentifier PieceIdentifier() { return CAISSA_PIECEID_INVALID; }
        virtual CaissaPieceIdentifier GetPieceIdentifier() const { return CaissaPiece::PieceIdentifier(); }

        CaissaTeamIdentifier GetTeamIdentifier() const { return m_TeamIdentifier; }
    
    private:
        CaissaTeamIdentifier m_TeamIdentifier;
};