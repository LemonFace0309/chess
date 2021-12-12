#include <iostream>
#include <vector>

#include "textdisplay.h"
#include "display.h"
#include "square.h"
#include "piece.h"
#include "pieceEnum.cc"

TextDisplay::TextDisplay(int cols, int rows) : rows{rows}, cols{cols} {}

void TextDisplay::notify(Square * square) {
  Piece *piece = square->getPiece();
  int row = square->getRow();
  int col = square->getCol();
  display[col][row] = piece->getPieceType();
}

void TextDisplay::render() {
  for (int col = 1; col <= cols; ++col) {
    for (int row = 1; row <= rows; ++rows) {
      const PieceEnum pieceEnum = display[col][row];
      //do later
    }
  }
}

TextDisplay::~TextDisplay() {}