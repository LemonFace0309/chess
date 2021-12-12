#ifndef _PAWN_H_
#define _PAWN_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "pieceEnum.cc"

class Pawn : public Piece {
  public:
    PieceEnum getPieceType();
    std::vector<std::string> getValidMoves(std::string coord);
};
#endif
