#ifndef _KING_H_
#define _KING_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "pieceEnum.cc"

class King : public Piece {
  public:
    PieceEnum getPieceType();
    std::vector<std::string> getValidMoves(std::string coord);
};
#endif
