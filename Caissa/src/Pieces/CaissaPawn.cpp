#include "Pieces/CaissaPawn.h"

std::vector<CaissaMove> CaissaPawn::GetPseudolegalMoves()
{
	std::vector<CaissaMove> moves;
	moves.push_back(CaissaMove(0, 0, 0, 0));
	return moves;
}