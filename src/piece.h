#include <iostream>
#include <vector>

#include "colourEnum.cc"

class Piece {
  private:
    ColourEnum colour;
  public:
    Piece(ColourEnum colour);
    virtual std::vector<std::string> getValidMoves(std::string coord) = 0;
};
