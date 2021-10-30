#include "Pieces/CaissaPawn.h"

CaissaPawn::CaissaPawn(CaissaTeamIdentifier teamIdentifier)
	: CaissaPiece(teamIdentifier), m_HasMoved(false)
{

}

std::vector<CaissaMove> CaissaPawn::GetPseudolegalMoves(std::shared_ptr<CaissaConfiguration> configuration, uint16_t rank, uint16_t file)
{
	std::vector<CaissaMove> moves;

	if (/* not blocked */)
	{
		moves.push_back(CaissaMove(rank, file, rank, file + 1));
	}

	if (!m_HasMoved)
	{
		moves.push_back(CaissaMove(rank, file, rank, file + 2));
	}

	return moves;
}