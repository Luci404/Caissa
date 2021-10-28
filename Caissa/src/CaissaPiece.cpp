#include "CaissaPiece.h"

CaissaPiece::CaissaPiece(CaissaTeamIdentifier teamIdentifier)
	: m_TeamIdentifier(teamIdentifier)
{   
}

std::vector<CaissaMove> CaissaPiece::GetPseudolegalMoves()
{
	return std::vector<CaissaMove>();
}