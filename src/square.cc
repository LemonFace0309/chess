#include <iostream>
#include <memory>

#include "square.h"
#include "piece.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"
#include "pawn.h"
using namespace std;


Square::Square(int row, int col) : row{row}, col{col} {};


class InvalidPiece {

};

void Square::setPiece(PieceEnum p, bool isWhiteTurn) {
  Colour colour = isWhiteTurn ? Colour::White : Colour::Black;

  switch (P) {
    case K:
      piece = make_unique<King>(colour);
      break;
    case Q:
      piece = make_unique<Queen>(colour);
      break;
    case B:
      piece = make_unique<Bishop>(colour);
      break;
    case R:
      piece = make_unique<Rook>(colour);
      break;
    case N:
      piece = make_unique<Knight>(colour);
      break;
    case P:
      piece = make_unique<Pawn>(colour);
      break;
    default:
      throw InvalidPiece();
  }
}