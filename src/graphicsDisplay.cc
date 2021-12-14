#include <iostream>
#include <vector>

#include "GraphicsDisplay.h"
#include "square.h"
#include "board.h"
#include "piece.h"
#include "pieceEnum.cc"
using namespace std;

GraphicsDisplay::GraphicsDisplay(Board *subject, int cols, int rows) : subject{subject}, rows{rows}, cols{cols} {}

void GraphicsDisplay::notify() {
  Square *changedSquare = subject->getRecentSquareWithAction();
  Piece *piece = changedSquare->getPiece();
  int row = changedSquare->getRow();
  int col = changedSquare->getCol();
  display[col][row] = piece->getPieceType();
}

void GraphicsDisplay::render() {
  for (int row = rows; row >= 1; --rows) {
    cout << row << " "; // row coordinate
    for (int col = 1; col <= cols; ++col) {
      const PieceEnum pieceEnum = display[col][row];
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

GraphicsDisplay::~GraphicsDisplay() {}