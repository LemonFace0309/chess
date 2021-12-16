#include <iostream>
#include <vector>

#include "textdisplay.h"
#include "square.h"
#include "board.h"
#include "piece.h"
#include "pieceEnum.cc"
using namespace std;

// Constructor
TextDisplay::TextDisplay(Board *subject, int cols, int rows) : subject{subject}, rows{rows}, cols{cols} {
  for (char col = 1; col <= cols; ++col) {
    vector<PieceEnum> column;
    for (int row = 1; row <= rows; ++row) {
      column.push_back(PieceEnum::NONE);
    }
    display.push_back(column);
  }
}

// Notifies the observer
void TextDisplay::notify() {
  Square *changedSquare = subject->getRecentSquareWithAction();
  Piece *piece = changedSquare->getPiece();
  int row = changedSquare->getRow();
  int col = changedSquare->getCol();
  display[col - 1][row - 1] = piece ? piece->getPieceType() : PieceEnum::NONE;
}

// Renders the game as text
void TextDisplay::render() {
  for (int row = rows; row >= 1; --row) {
    // Prints the row coordinates
    cout << row << " ";
    // Draws the board's row
    for (int col = 1; col <= cols; ++col) {
      const PieceEnum pieceEnum = display[col - 1][row - 1];
      // Outputs the piece or square
      switch(pieceEnum) {
        case k: // black pieces
          cout << "k";
          break;
        case q:
          cout << "q";
          break;
        case b:
          cout << "b";
          break;
        case r:
          cout << "r";
          break;
        case n:
          cout << "n";
          break;
        case p:
          cout << "p";
          break;
        case K: // white pieces
          cout << "K";
          break;
        case Q:
          cout << "Q";
          break;
        case B:
          cout << "B";
          break;
        case R:
          cout << "R";
          break;
        case N:
          cout << "N";
          break;
        case P:
          cout << "P";
          break;
        default:
          // determines is square is white or black
          if ((row + col) % 2 == 0) {
            cout << "_";
          } else {
            cout << " ";
          }
      }
    }
    cout << endl;
  }
  cout << endl;

  // prints column coordinates
  cout << "  ";
  for (int col = 1; col <= cols; ++col) {
    char coord = 97 + col - 1;
    cout << coord;
  }
  cout << endl;
  cout << endl;
}

// Destructor
TextDisplay::~TextDisplay() {}