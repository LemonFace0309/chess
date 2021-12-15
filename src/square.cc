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

// Constructor
Square::Square(int col, int row) : col{col}, row{row} {};

// Returns the row the square is in
int Square::getRow() {
  return row;
}

// Returns the column the square is in
int Square::getCol() {
  return col;
}

// Returns the piece on the square
Piece *Square::getPiece() {
  return piece.get();
}

class InvalidPiece {

};

// Sets a piece onto the square
void Square::setPiece(PieceEnum p, bool isWhiteTurn) {
  ColourEnum colour = isWhiteTurn ? ColourEnum::White : ColourEnum::Black;
  // Changes the current piece on the square into the new piece
  switch (P) {
    case PieceEnum::K:
      piece = make_unique<King>(colour);
      break;
    case PieceEnum::Q:
      piece = make_unique<Queen>(colour);
      break;
    case PieceEnum::B:
      piece = make_unique<Bishop>(colour);
      break;
    case PieceEnum::R:
      piece = make_unique<Rook>(colour);
      break;
    case PieceEnum::N:
      piece = make_unique<Knight>(colour);
      break;
    case PieceEnum::P:
      piece = make_unique<Pawn>(colour);
      break;
    case PieceEnum::NONE:
      piece = nullptr;
      break;
    default:
      throw InvalidPiece();
  }
}