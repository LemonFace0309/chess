#include <iostream>
#include <vector>

#include "piece.h"

class Rook : public Piece {
  public:
    std::vector<std::string> getValidMoves(std::string coord);
};