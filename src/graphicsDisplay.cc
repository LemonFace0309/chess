#include <iostream>
#include <vector>

#include "window.h"
#include "GraphicsDisplay.h"
#include "square.h"
#include "board.h"
#include "piece.h"
#include "pieceEnum.cc"
using namespace std;

GraphicsDisplay::GraphicsDisplay(Board *subject, int cols, int rows)
 : subject{subject}, 
   rows{rows}, 
   cols{cols} {}

void GraphicsDisplay::notify() {
  Square *changedSquare = subject->getRecentSquareWithAction();
  Piece *piece = changedSquare->getPiece();
  int row = changedSquare->getRow();
  int col = changedSquare->getCol();
  display[col][row] = piece->getPieceType();
}

void GraphicsDisplay::render() {
    // Sets the x and y coordinates. It starts at 20 for a 20px margin around the game
    int x = 20;
    int y = 20;
    for (int row = rows; row >= 1; --rows) {
    // Draws the row labels
    window->drawString(x, y, row, Xwindow::Black);

    // Draws the board
    x = 60;
    for (int col = 1; col <= cols; ++col) {
      const PieceEnum pieceEnum = display[col][row];

      // Determines is square is white or black
      bool isBlackSquare = (row + col) % 2 == 0;

      // Sets the square coloring and changes the piece's font color depending
      //   on if the square is black or not
      int squareColor;
			int pieceColor;
      if (isBlackSquare) {
				squareColor = Xwindow::Black;
				pieceColor = Xwindow::White;
      } else {
				squareColor = Xwindow::White;
				pieceColor = Xwindow::Black;
			}

      // Draws the Square
			window->drawSquare(x, y, squareColor);

      // Draws the piece (string)
      switch(pieceEnum) {
        case K:
					window->drawString(x, y, "K", pieceColor);
          cout << "k";
          break;
        case Q:
          window->drawString(x, y, "Q", pieceColor);
          break;
        case B:
          window->drawString(x, y, "B", pieceColor);
          break;
        case R:
          window->drawString(x, y, "R", pieceColor);
          break;
        case N:
          window->drawString(x, y, "N", pieceColor);
          break;
        case P:
          window->drawString(x, y, "P", pieceColor);;
          break;
				case k:
          window->drawString(x, y, "k", pieceColor);
          break;
        case q:
          window->drawString(x, y, "q", pieceColor);
          break;
        case b:
          window->drawString(x, y, "b", pieceColor);
          break;
        case r:
          window->drawString(x, y, "r", pieceColor);
          break;
        case n:
          window->drawString(x, y, "n", pieceColor);
          break;
        case p:
          window->drawString(x, y, "p", pieceColor);
          break;	
        default:
					break;
      }
			x += 20
    }
    x = 20;
		y += 20;
  }
  x = 60;
	y += 40;

  // Draws column coordinates
  for (int col = 1; col <= cols; ++col) {
    char coord = 97 + col - 1;
    window->drawString(x, y, coord, Xwindow::Black);
  }
  x = 20;
	y += 20;
}

GraphicsDisplay::~GraphicsDisplay() {}