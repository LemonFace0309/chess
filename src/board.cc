#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "board.h"
#include "textdisplay.h"
#include "graphicsDisplay.h"
#include "square.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
using namespace std;

Board::Board(const int rows, const int cols) : rows{rows}, cols{cols} {
  shared_ptr<TextDisplay> td = make_shared<TextDisplay>(this, cols, rows);
  attach(td);
  // shared_ptr<GraphicsDisplay> gd = make_shared<GraphicsDisplay>(this, cols, rows);
  // attach(gd);

  for (char col = 'a'; col < cols + 97; ++col) {
    for (int row = 1; row <= rows; ++row) {
      const string coord = string(1, col) + to_string(row);
      squares[coord] = make_unique<Square>(col - 97 + 1, row);
    }
  }
}

Square *Board::getRecentSquareWithAction() {
  return squares[lastAction].get();
}

bool Board::setSquare(PieceEnum p, bool isWhiteTurn, string coord, bool firstTurn) {
  if (!squares[coord]) return false; // invalid coordinate

  squares[coord]->setPiece(p, isWhiteTurn, firstTurn);
  lastAction = coord;
  notifyObservers();
  return true;
}

void Board::finishTurn() {
  renderObservers();
}

vector<string> Board::getValidMoves() {
  vector<string> validMoves;
  ColourEnum turnColour;
  if (isWhiteTurn) {
    turnColour = ColourEnum::White;
  } else {
    turnColour = ColourEnum::Black;
  }
  for (char col = 'a'; col < cols + 97; ++col) {
    for (int row = 1; row <= rows; ++row) {
      const string coord = string(1, col) + to_string(row);
      Piece *piece = squares[coord].getPiece();
      if (piece != nullptr && piece.getColour() == turnColour) {
        const PieceEnum pieceEnum = piece->getPieceType();
        vector<vector<string>> allMoves = piece->getValidMoves();
        switch(pieceEnum) {
        case Q:
        case q:
        case R:
        case r:
        case B:
        case b:          
          for (auto moves : allMoves) {
            for (auto move : moves) {
              Piece *othPiece = squares[move].getPiece();
              if (othPiece != nullptr) {
                if (othPiece.getColour() != turnColour) {
                  validMoves.emplace_back(move);
                }
                break;
              } else {
                validMoves.emplace_back(move);
              }
            }
          }
          break;
        case N:
        case n:
          for (auto moves : allMoves) {
            validMoves.insert(validMoves.end(), moves.begin(), moves.end());
          }
          break;
        case P:
        case p:
          for (auto moves : allMoves) {
            for (auto move : moves) {
              Piece *othPiece = squares[move].getPiece();
              if (othPiece == nullptr) {
                validMoves.emplace_back(move);
              }
            }
          }
          break;
        case K:
        case k:
          for (auto moves : allMoves) {
            for (auto move : moves) {
              Piece *othPiece = squares[move].getPiece();
              if (othPiece != nullptr) {
                if (othPiece.getColour() != turnColour) {
                  validMoves.emplace_back(move);
                }
              } else {
                validMoves.emplace_back(move);
              }
            }
          }
          break;
        }
      }
    }
  }
  return validMoves;
}