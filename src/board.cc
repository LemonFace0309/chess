#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <sstream>

#include "board.h"
#include "textdisplay.h"
#include "graphicsDisplay.h"
#include "square.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"
#include "pawn.h"
using namespace std;

Board::Board(const int rows, const int cols) : rows{rows}, cols{cols} {
  whiteKingCoord = "";
  blackKingCoord = "";
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

  if (p == PieceEnum::k) {
    blackKingCoord = coord;
  } else if (p == PieceEnum::K) {
    whiteKingCoord = coord;
  }
  squares[coord]->setPiece(p, isWhiteTurn, firstTurn);
  lastAction = coord;
  notifyObservers();
  return true;
}

void Board::finishTurn() {
  renderObservers();
}


vector<string> filterKingMoves(vector<string> validKingMoves, vector<string> validOpponentMoves) {
  vector<string> filteredMoves;
  for (auto move : validKingMoves) {
    istringstream coords{ move };
    string initCoord;
    string finalCoord;
    coords >> initCoord;
    coords >> finalCoord;
    for (auto oMoves : validOpponentMoves) {
      if (oMoves.find(finalCoord, 3) == -1) {
        filteredMoves.emplace_back(move);
      }
    }
  }
  return filteredMoves;
}

void addMove(string coord, bool pieceIsWhite, vector<string> &validWhiteMoves, 
              vector<string> &validBlackMoves) {
  if (pieceIsWhite) {
    validWhiteMoves.emplace_back(coord);
  } else {
    validBlackMoves.emplace_back(coord);
  } 
}

void Board::flattenMoves(string coord, PieceEnum pieceEnum, vector<vector<string>> allMoves,
                   ColourEnum other, vector<string> &validWhiteMoves, vector<string> &validBlackMoves) {
  bool pieceIsWhite = other == ColourEnum::White;
  if (pieceEnum == PieceEnum::Q || pieceEnum == PieceEnum::q || 
    pieceEnum == PieceEnum::Q || pieceEnum == PieceEnum::q || 
    pieceEnum == PieceEnum::Q || pieceEnum == PieceEnum::q) {
    // Iterates through all the possible moves    
    for (auto moves : allMoves) {
      for (auto move : moves) {
        Piece *othPiece = squares[move]->getPiece();
        // Checks if the square is empty
        if (othPiece != nullptr) {
            // Checks if the piece at the square is takeable
            if (othPiece->getColour() != other) {
              addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);                  
            }
            break;
          } else {
            addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);     
          }
        }
      }
  } else if (pieceEnum == PieceEnum::N || pieceEnum == PieceEnum::n ) {
    // Iterates through all the possible moves
    for (auto moves : allMoves) {
      for (auto move : moves) { 
        Piece *othPiece = squares[move]->getPiece();
        // Checks if the square is empty
        if (othPiece != nullptr) {
          // Checks if the piece at the square is takeable
          if (othPiece->getColour() != other) {
            addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);                  
          }
        } else {
          addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);     
        }
      }     
    }    
  } else if (pieceEnum == PieceEnum::P || pieceEnum == PieceEnum::p ) {
    for (auto moves : allMoves) {
      for (auto move : moves) {
        Piece *othPiece = squares[move]->getPiece();
        // Checks if the square is empty
        if (othPiece == nullptr) {
          addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);    
        }
      }
    }
    // Checks if the pawn can take any pieces
    int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
    int y = coord[1] - 49 + 1; // 1, 2, ..., 8
    vector<string> tempCoords;
    tempCoords.emplace_back(string(1, char(x + 96) - 1) + to_string(y + 1));
    tempCoords.emplace_back(string(1, char(x + 96) + 1) + to_string(y + 1));
    for (auto coords : tempCoords) {
      Square *square = squares[coord];
      if (square != nullptr) {
        Piece *tempPiece = square->getPiece();
        if (tempPiece != nullptr) {
          if (tempPiece->getColour() != other) {
            addMove(coord + " " + coords, pieceIsWhite, validWhiteMoves, validBlackMoves);                  
          }      
        }
      }
    }
  }
}


bool isValidMove(string coord1, string coord2, bool firstTurn) {
  string colourTurn;
  ColourEnum colour;
  if (isWhiteTurn) {
    colourTurn = "white";
    colour = colourEnum::White;
  } else {
    colourTurn = "black";
    colour = colourEnum::Black;
  }
  if (find(allMoves[colourTurn].begin(), allMoves[colourTurn].end(), item) 
      != allMoves[colourTurn].end()) {
    PieceEnum oldP;
    Piece * oldPiece = squares[coord1]->getPiece();
    if (oldPiece == nullptr) {
      oldP = PieceEnum::NONE;
    } else {
      oldP = oldPiece.getPieceType();
    }        
    PieceEnum p = squares[coord1]->getPiece()->getPieceType();
    squares[coord2]->setPiece(p, isWhiteTurn, firstTurn);
    if (isWhiteTurn && isChecked(isWhiteTurn)) {
      squares[coord1]->setPiece(p, isWhiteTurn, firstTurn);
      squares[coord2]->setPiece(oldP, isWhiteTurn, firstTurn);
      return false;
    } else if (!isWhiteTurn && isChecked(!isWhiteTurn)) {
      squares[coord1]->setPiece(p, isWhiteTurn, firstTurn);
      squares[coord2]->setPiece(oldP, isWhiteTurn, firstTurn);
      return false;
    }
    return true;
  }
  return false;
}

// Checks if the black or white king is being checked
bool isChecked(bool isWhiteChecked) {
  // Gets the coordinates of the black or white king
  string coord;
  if (isWhiteChecked) {
    coord = whiteKingCoord;
  } else {
    coord = blackKingCoord;
  }
  // Gets the coordinates of possible checks by a knight
  vector<vector<string>> Knight::getValidMoves(string coord, cols, rows, false);
  for (auto coords : knightChecks) {
    for (auto coord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[coord];
      if (square != nullptr) {
        // Checks if the piece at the coordinate is an opposing knight
        Piece * piece = square->getPiece();
        if (piece != nullptr && piece->getColour() != colour) {
          if (isWhiteChecked && piece->getPieceType() == PieceEnum::n) {
            return true;
          } else if (!isWhiteChecked && piece->getPieceType() == PieceEnum::N) {
            return true;  
          }             
        }   
      }
    }
  }
  // Gets the coordinates of possible checks of the row and col the king is at
  vector<vector<string>> vertHorzChecks = Rook::getValidMoves(coord, cols, rows, false);
  for (auto coords : vertHorzChecks) {
    for (auto coord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[coord];
      if (square != nullptr) {
        Piece * piece = square->getPiece();
        // Checks if the piece at the coordinate is an opposing rook or queen, since only
        //   rooks and queens can move horizontally/vertically
        if (piece != nullptr && piece->getColour() != colour) {
          if (isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::r || piece->getPieceType() == PieceEnum::q)
              return true;
          } else if (!isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::R || piece->getPieceType() == PieceEnum::Q)
              return true;
          }             
        }   
      }
    }
  }
  // Gets the coordinates of possible checks of the diagonals the king is at
  vector<vector<string>> diagonalChecks = Bishop::getValidMoves(coord, cols, rows, false);
  for (auto coords : diagonalChecks) {
    for (auto coord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[coord];
      if (square != nullptr) {
        // Checks if the piece at the coordinate is an opposing bishop or queen, since only
        //   bishops and queens can move diagonally
        Piece * piece = square->getPiece();
        if (piece != nullptr && piece->getColour() != colour) {
          if (isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::b || piece->getPieceType() == PieceEnum::q)
              return true;
          } else if (!isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::B || piece->getPieceType() == PieceEnum::Q)
              return true;
          }             
        }   
      }
    }
  }
  // Gets the coordinates of possible checks  by pawns
  int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
  int y = coord[1] - 49 + 1; // 1, 2, ..., 8
  vector<string> tempCoords;
  tempCoords.emplace_back(string(1, char(x + 96) - 1) + to_string(y - 1));
  tempCoords.emplace_back(string(1, char(x + 96) + 1) + to_string(y - 1));
  for (auto coords : tempCoords) {
    // Checks if the square exists on the board
    Square *square = squares[coord];
    if (square != nullptr) {
      // Checks if the piece at the coordinate is an opposing pawn
      Piece *piece = square->getPiece();
      if (piece != nullptr && piece->getColour() != colour) {
        if (isWhiteChecked) {
          if (piece->getPieceType() == PieceEnum::p)
            return true;
        } else if (!isWhiteChecked) {
          if (piece->getPieceType() == PieceEnum::P)
            return true;
        }    
      }
    }
  }
  return false;
}

void Board::findAllValidMoves(bool firstTurn) {
  vector<string> validWhiteMoves;
  vector<string> validWhiteKingMoves;
  vector<string> validBlackMoves; 
  vector<string> validBlackKingMoves; 
  // vector<string> whiteChecksOnBlack;
  // vector<string> blackChecksOnWhite;
  // Determines which colour's turn it is
  ColourEnum turnColour;
  if (isWhiteTurn) {
    turnColour = ColourEnum::White;
  } else {
    turnColour = ColourEnum::Black;
  }
  // Iterates through every square on the board
  for (char col = 'a'; col < cols + 97; ++col) {
    for (int row = 1; row <= rows; ++row) {
      // Gets the coordinate of the square
      const string coord = string(1, col) + to_string(row);
      // Checks if there's a piece on the square
      Piece *piece = squares[coord]->getPiece();
      if (piece != nullptr) {
        bool pieceIsWhite = piece->getColour() == ColourEnum::White;
        const PieceEnum pieceEnum = piece->getPieceType();
        // Gets all the potential moves of the piece at coord
        vector<vector<string>> allMoves = piece->getValidMoves(coord, cols, rows, firstTurn);
        // Filters out all the non-valid moves
        flattenMoves(coord, pieceEnum, allMoves, piece->getColour(), validWhiteMoves, validBlackMoves);
        if (pieceEnum == PieceEnum::K || pieceEnum == PieceEnum::k ) {
          // Iterates through all the possible moves
          for (auto moves : allMoves) {
            for (auto move : moves) {
              Piece *othPiece = squares[move]->getPiece();
              // Checks if the square is empty
              if (othPiece != nullptr) {
                // Checks if the piece at the square is takeable
                if (othPiece->getColour() != turnColour) {
                  addMove(coord + " " + move, pieceIsWhite, validWhiteKingMoves, validBlackKingMoves);    
                }
              } else {
                addMove(coord + " " + move, pieceIsWhite, validWhiteKingMoves, validBlackKingMoves);    
              }
            }
          }
        }
      }
    }
  }
  // Filters out any moves that will put their own king in check
  validWhiteKingMoves = filterKingMoves(validWhiteKingMoves, validBlackMoves);
  validBlackKingMoves = filterKingMoves(validBlackKingMoves, validWhiteMoves);
  validWhiteMoves.insert(validWhiteMoves.end(), validWhiteKingMoves.begin(), validWhiteKingMoves.end());
  validBlackMoves.insert(validBlackMoves.end(), validBlackKingMoves.begin(), validBlackKingMoves.end());

  allValidMoves["white"] = validWhiteMoves;
  allValidMoves["black"] = validBlackMoves;
}
