#ifndef _ROOK_H_
#define _ROOK_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "pieceEnum.cc"

class Rook : public Piece {
  public:
    PieceEnum getPieceType();
    std::vector<std::string> getValidMoves(std::string coord);
};
#endif
