#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "board.h"
#include "textdisplay.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
#include "square.h"
using namespace std;

Board::Board(const int rows, const int cols) : rows{rows}, cols{cols} {
  unique_ptr<TextDisplay> td = make_unique<TextDisplay>(this, cols, rows);
  attach(move(td));

  for (int row = 1; row <= rows; ++row) {
    for (char col = 'a'; col <= cols + 97; ++col) {
      const string coord = to_string(col) + to_string(row);
      squares[coord] = make_unique<Square>(coord, true);
    }
  }
}

Square *Board::getRecentSquareWithAction() {
  return squares[lastAction].get();
}

bool Board::setSquare(PieceEnum p, bool isWhiteTurn, string coord) {
  if (!squares[coord]) return false; // invalid coordinate

  squares[coord]->setPiece(p, isWhiteTurn);
  lastAction = coord;
  notifyObservers();
  return true;
}

void Board::finishTurn() {
  renderObservers();
}
