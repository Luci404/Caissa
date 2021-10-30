#include "CaissaPiece.h"

CaissaPiece::CaissaPiece(CaissaTeamIdentifier teamIdentifier)
	: m_TeamIdentifier(teamIdentifier)
{   
}

std::vector<CaissaMove> CaissaPiece::GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file)
{
	return std::vector<CaissaMove>();
}