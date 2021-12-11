#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "board.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
#include "square.h"
using namespace std;

Board::Board(const int rows, const int cols) : rows{rows}, cols{cols} {
  for (int row = 1; row <= rows; ++row) {
    for (char col = 'a'; col <= cols + 97; ++col) {
      const string coord = to_string(col) + to_string(row);
      squares[coord] = make_unique<Square>(coord, true);
    }
  }
}

bool Board::setSquare(PieceEnum p, bool isWhiteTurn, string coord) {
  if (!squares[coord]) return false; // invalid coordinate

  squares[coord]->setPiece(p, isWhiteTurn);
  return true;
}
