#ifndef _QUEEN_H_
#define _QUEEN_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "pieceEnum.cc"

class Queen : public Piece {
  public:
    PieceEnum getPieceType();
    std::vector<std::string> getValidMoves(std::string coord);
};
#endif
