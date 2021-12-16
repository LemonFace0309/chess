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
Square::Square(int col, int row) : col{col}, row{row}, firstMove{true} {};

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

bool Square::isFirstTurn() {
  return firstMove;
}

// Sets a piece onto the square
void Square::setPiece(PieceEnum p, bool isWhiteTurn, bool firstTurn) {
  firstMove = firstTurn;
  ColourEnum colour = isWhiteTurn ? ColourEnum::White : ColourEnum::Black;
  // Changes the current piece on the square into the new piece
  switch (p) {
    case PieceEnum::K:
    case PieceEnum::k:
      piece = make_unique<King>(colour);
      break;
    case PieceEnum::Q:
    case PieceEnum::q:
      piece = make_unique<Queen>(colour);
      break;
    case PieceEnum::B:
    case PieceEnum::b:
      piece = make_unique<Bishop>(colour);
      break;
    case PieceEnum::R:
    case PieceEnum::r:
      piece = make_unique<Rook>(colour);
      break;
    case PieceEnum::N:
    case PieceEnum::n:
      piece = make_unique<Knight>(colour);
      break;
    case PieceEnum::P:
    case PieceEnum::p:
      piece = make_unique<Pawn>(colour);
      break;
    case PieceEnum::NONE:
      piece = nullptr;
      break;
    default:
      throw InvalidPiece();
  }
}