#include <iostream>
#include <vector>

#include "piece.h"

class King : public Piece {
  public:
    std::vector<std::string> getValidMoves(std::string coord);
};