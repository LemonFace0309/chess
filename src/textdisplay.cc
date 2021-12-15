#include <iostream>
#include <vector>

#include "textdisplay.h"
#include "square.h"
#include "board.h"
#include "piece.h"
#include "pieceEnum.cc"
using namespace std;

// Constructor
TextDisplay::TextDisplay(Board *subject, int cols, int rows) : subject{subject}, rows{rows}, cols{cols} {}

// Notifies the observer
void TextDisplay::notify() {
  Square *changedSquare = subject->getRecentSquareWithAction();
  Piece *piece = changedSquare->getPiece();
  int row = changedSquare->getRow();
  int col = changedSquare->getCol();
  display[col][row] = piece->getPieceType();
}

// Renders the game as text
void TextDisplay::render() {
  for (int row = rows; row >= 1; --rows) {
    // Prints the row coordinates
    cout << row << " ";
    // Draws the board's row
    for (int col = 1; col <= cols; ++col) {
      const PieceEnum pieceEnum = display[col][row];
      // Outputs the piece or square
      switch(pieceEnum) {
        case K:
          cout << "k";
          break;
        case Q:
          cout << "q";
          break;
        case B:
          cout << "b";
          break;
        case R:
          cout << "r";
          break;
        case N:
          cout << "n";
          break;
        case P:
          cout << "p";
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
  for (int col = 1; col <= cols; ++col) {
    char coord = 97 + col - 1;
    cout << coord;
  }
  cout << endl;
}

// Destructor
TextDisplay::~TextDisplay() {}