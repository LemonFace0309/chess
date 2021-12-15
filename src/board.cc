#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>

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
  isWhiteTurn = true;
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

  if (p == PieceEnum::K && !isWhiteTurn) {
    blackKingCoord = coord;
  } else if (p == PieceEnum::K && isWhiteTurn) {
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

// Flattens the vector of vector of coordinates into a single vector
void Board::flattenMoves(string coord, PieceEnum pieceEnum, vector<vector<string>> allMoves,
                   ColourEnum other, vector<string> &validWhiteMoves, vector<string> &validBlackMoves) {
  bool pieceIsWhite = other == ColourEnum::White;
  if (pieceEnum == PieceEnum::R || pieceEnum == PieceEnum::r || 
    pieceEnum == PieceEnum::B || pieceEnum == PieceEnum::b || 
    pieceEnum == PieceEnum::Q || pieceEnum == PieceEnum::q) {
    // Iterates through all the possible moves    
    for (auto moves : allMoves) {
      for (auto move : moves) {
        Piece *othPiece = squares[move]->getPiece();
        // Checks if the square is empty
        if (othPiece != nullptr) {
            // Checks if the piece at the square is takeable
            if (othPiece->getColour() != other) {
              // cout << coord << " " << move << endl;
              addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);                  
            }
            break;
          } else {
            // cout << coord << " " << move << endl;
            addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);     
          }
        }
      }
  } else if (pieceEnum == PieceEnum::N || pieceEnum == PieceEnum::n ) {
    // Iterates through all the possible moves
    for (auto moves : allMoves) {
      for (auto move : moves) { 
        Square *square = squares[move].get();
        if (square != nullptr) {
          Piece *othPiece = squares[move]->getPiece();
          // Checks if the square is empty
          if (othPiece != nullptr) {
            // Checks if the piece at the square is takeable
            if (othPiece->getColour() != other) {
              // cout << coord << " " << move << endl;
              addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);                  
            }
            
          } else {
            // cout << coord << " " << move << endl;
            addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);     
          }
        }   
      }  
    }    
  } else if (pieceEnum == PieceEnum::P || pieceEnum == PieceEnum::p ) {
    for (auto moves : allMoves) {
      for (auto move : moves) {
        Piece *othPiece = squares[move]->getPiece();
        // Checks if the square is empty
        if (othPiece == nullptr) {
          // cout << coord << " " << move << endl;
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
      Square *square = squares[coords].get();
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

// Flattens the vector of vector of coordinates into a single vector, which contains all the possible
//   moves to stop the check
void Board::flattenCheckedMoves(string coord, PieceEnum pieceEnum, vector<vector<string>> allMoves, vector<string> validCheckMoves,
                   ColourEnum other, vector<string> &validWhiteMoves, vector<string> &validBlackMoves) {
  bool pieceIsWhite = other == ColourEnum::White;
  if (pieceEnum == PieceEnum::R || pieceEnum == PieceEnum::r || 
    pieceEnum == PieceEnum::B || pieceEnum == PieceEnum::b || 
    pieceEnum == PieceEnum::Q || pieceEnum == PieceEnum::q) {
    // Iterates through all the possible moves    
    for (auto moves : allMoves) {
      for (auto move : moves) {
        Piece *othPiece = squares[move]->getPiece();
        // Sees if the move is a move that can stop the check
        if (find(validCheckMoves.begin(), validCheckMoves.end(), move) != validCheckMoves.end()) {
          if (othPiece != nullptr) {
            // Sees if we can take the piece
            if (othPiece->getColour() != other) {
              // cout << coord << " " << move << endl;
              addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);             
            }
            break;
          } else {
            // cout << coord << " " << move << endl;
            addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);
            break;     
          }
        }        
      }
    }
  } else if (pieceEnum == PieceEnum::N || pieceEnum == PieceEnum::n ) {
    // Iterates through all the possible moves
    for (auto moves : allMoves) {
      for (auto move : moves) { 
        Square *square = squares[move].get();
        if (square != nullptr) {
          Piece *othPiece = squares[move]->getPiece();
          // Sees if the move is a move that can stop the check
          if (find(validCheckMoves.begin(), validCheckMoves.end(), move) != validCheckMoves.end()) {
            if (othPiece != nullptr) {
              // Sees if we can take the piece
              if (othPiece->getColour() != other) {
                // cout << coord << " " << move << endl;
                addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);             
              }
              break;
            } else {
              // cout << coord << " " << move << endl;
              addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);
              break;     
            }
          } 
        }   
      }  
    }    
  } else if (pieceEnum == PieceEnum::P || pieceEnum == PieceEnum::p ) {
    for (auto moves : allMoves) {
      for (auto move : moves) {
        Piece *othPiece = squares[move]->getPiece();
        /// Sees if the move is a move that can stop the check
        if (find(validCheckMoves.begin(), validCheckMoves.end(), move) != validCheckMoves.end()) {
          if (othPiece == nullptr)  {
            // cout << coord << " " << move << endl;
            addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);               
          }
          break;  
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
      Square *square = squares[coords].get();
      if (square != nullptr) {
        Piece *tempPiece = square->getPiece();
        // Sees if the move is a move that can stop the check
        if (find(validCheckMoves.begin(), validCheckMoves.end(), coords) != validCheckMoves.end()) {
          if (tempPiece != nullptr) {
            if (tempPiece->getColour() != other) {
              // cout << coord << " " << coords << endl;
              addMove(coord + " " + coords, pieceIsWhite, validWhiteMoves, validBlackMoves);             
            }
          }
        }
      }
    }
  }
}

void Board::move(string coord1, string coord2, bool firstTurn) {
   cout << "isMoving" << endl;
  PieceEnum p = squares[coord1]->getPiece()->getPieceType();
  Board::setSquare(p, isWhiteTurn, coord2, firstTurn);
  Board::setSquare(PieceEnum::NONE, isWhiteTurn, coord1, firstTurn);
  isWhiteTurn = !isWhiteTurn;
}

// Checks if the move is valid
bool Board::isValidMove(string coord1, string coord2, bool firstTurn) {
  string colourTurn;
  ColourEnum colour;
  cout << "isValidMove" << endl;
  if (isWhiteTurn) {
    colourTurn = "white";
    colour = ColourEnum::White;
  } else {
    colourTurn = "black";
    colour = ColourEnum::Black;
  }
  string move = coord1 + " " + coord2;
  // cout << move << endl;
  //  for (auto move : allValidMoves[colourTurn]) {
  //   cout << move << endl;
  // }
  // Checks if the move is valid (doesn't include being under check)
  if (find(allValidMoves[colourTurn].begin(), allValidMoves[colourTurn].end(), move)
      != allValidMoves[colourTurn].end()) {
        //cout << "Keeps track of the old piece at the square in case the move isn't v" << endl;
    // Keeps track of the old piece at the square in case the move isn't valid
    PieceEnum oldP;
    Piece * oldPiece = squares[coord1]->getPiece();
    if (oldPiece == nullptr) {
      oldP = PieceEnum::NONE;
    } else {
      oldP = oldPiece->getPieceType();
    }        
    //cout << "rsbfsdbsdbfsdbfsb" << endl;
    // Temporarily moves the piece
    PieceEnum p = squares[coord1]->getPiece()->getPieceType();
    //cout << p << endl;
    squares[coord2]->setPiece(p, isWhiteTurn, firstTurn);
    // cout << "bbbbbbbbbbbbbbb" << endl;
    // Checks if the king is under check after the move
    if (isWhiteTurn && isChecked(isWhiteTurn) != "") {
      // Undos the move
      squares[coord1]->setPiece(p, isWhiteTurn, firstTurn);
      squares[coord2]->setPiece(oldP, isWhiteTurn, firstTurn);
      return false;
    } else if (!isWhiteTurn && isChecked(!isWhiteTurn) != "") {
      // Undos the move
      squares[coord1]->setPiece(p, isWhiteTurn, firstTurn);
      squares[coord2]->setPiece(oldP, isWhiteTurn, firstTurn);
      return false;
    }
    return true;
  }
  return false;
}

// Checks if the black or white king is being checked. Returns "" if the king isn't
//  being checks, and the coordinate if the piece that is checking the king if it is
string Board::isChecked(bool isWhiteChecked) {
  ColourEnum colour = ColourEnum::White;
  // Gets the coordinates of the black or white king
  string coord;
  // cout << "isChecked" << endl;
  if (isWhiteChecked) {
    coord = whiteKingCoord;
  } else {
    coord = blackKingCoord;
  }
  // Gets the coordinates of possible checks by a knight
  Knight knight{colour};
  vector<vector<string>> knightChecks = knight.getValidMoves(coord, cols, rows, false);
  for (auto coords : knightChecks) {
    for (auto coord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[coord].get();
      if (square != nullptr) {
        // Checks if the piece at the coordinate is an opposing knight
        Piece * piece = square->getPiece();
        if (piece != nullptr && piece->getColour() != colour) {
          if (isWhiteChecked && piece->getPieceType() == PieceEnum::n) {
            //cout << "n" << endl;
            return coord;
          } else if (!isWhiteChecked && piece->getPieceType() == PieceEnum::N) {
            //cout << "N" << endl;
            return coord;
          }             
        }   
      }
    }
  }
  // Gets the coordinates of possible checks of the row and col the king is at
  Rook rook{colour};
  vector<vector<string>> vertHorzChecks = rook.getValidMoves(coord, cols, rows, false);
  for (auto coords : vertHorzChecks) {
    for (auto coord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[coord].get();
      if (square != nullptr) {
        Piece * piece = square->getPiece();
        // Checks if the piece at the coordinate is an opposing rook or queen, since only
        //   rooks and queens can move horizontally/vertically
        if (piece != nullptr && piece->getColour() != colour) {
          if (isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::r || piece->getPieceType() == PieceEnum::q)
              return coord;
          } else if (!isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::R || piece->getPieceType() == PieceEnum::Q)
              return coord;
          }             
        }   
      }
    }
  }
  // Gets the coordinates of possible checks of the diagonals the king is at
  Bishop bishop{colour};
  vector<vector<string>> diagonalChecks = bishop.getValidMoves(coord, cols, rows, false);
  for (auto coords : diagonalChecks) {
    for (auto coord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[coord].get();
      if (square != nullptr) {
        // Checks if the piece at the coordinate is an opposing bishop or queen, since only
        //   bishops and queens can move diagonally
        Piece * piece = square->getPiece();
        if (piece != nullptr && piece->getColour() != colour) {
          if (isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::b || piece->getPieceType() == PieceEnum::q)
              return coord;
          } else if (!isWhiteChecked) {
            if (piece->getPieceType() == PieceEnum::B || piece->getPieceType() == PieceEnum::Q)
              return coord;
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
    Square *square = squares[coord].get();
    if (square != nullptr) {
      // Checks if the piece at the coordinate is an opposing pawn
      Piece *piece = square->getPiece();
      if (piece != nullptr && piece->getColour() != colour) {
        if (isWhiteChecked) {
          if (piece->getPieceType() == PieceEnum::p)
            return coord;
        } else if (!isWhiteChecked) {
          if (piece->getPieceType() == PieceEnum::P)
            return coord;
        }    
      }
    }
  }
  return "";
}

// Returns a vector of the coordinates that can help block/ stop the check
vector<string> Board::possibleUncheckMoves(string checkCoord, bool isWhiteChecked) {
  vector<string> possibleUncheckMoves;
  string coord;
  if (isWhiteChecked) {
    coord = whiteKingCoord;
  } else {
    coord = blackKingCoord;
  }
  // Gets the coordinates of the king and the pice thats checking the king
  int x1 = coord[0] - 97 + 1;
  int y1 = coord[1] - 49 + 1;
  int x2 = checkCoord[0] - 97 + 1; 
  int y2 = checkCoord[1] - 49 + 1;
  // if x1 == x2, then the king and the checking piece are on the same row, so we need to find
  //   all the coordinates between those 2 piece on that row, including the checking peice itself because
  //   taking the checking piece also stops the check
  if (x1 == x2) {
    // Makes y2 the bigger one
    if (y1 >= y2) {
      int temp = y1;
      int y1 = y2;
      int y2 = temp;
    }
    // Gets the coordinates between the 2 points
    for (int i = 1; y1 + i <= y2; i++) {
      possibleUncheckMoves.emplace_back(string(1, char(x1 + 96)) + to_string(y1 + i));
    }
  } else if (y1 == y2) {
    // if y1 == y2, then theyre on the same column
    // Makes x2 the bigger one
    if (x1 >= x2) {
      int temp = x1;
      int x1 = x2;
      int x2 = temp;
    }
    // gets the coordinates between the 2 pints
    for (int i = 1; x1 + i <= x2; i++) {
      possibleUncheckMoves.emplace_back(string(1, char(x1 + 96) + i) + to_string(y1));
    }
  } else if (abs(x1 - x2) == abs(y1 - y2)) {
    // This means that theyre on a diagonal
    int i1;
    int i2;
    // Calculates whether or not we need to subtract/ add from x1 and y1 
    if (x2 - x1 >= 0) {
      i1 = 1;
    } else {
      i1 = -1;
    }
    if (y2 - y1 >= 0) {
      i2 = 1;
    } else {
      i2 = -1;
    }
    // How long the loop runs for
    int stopLoop = abs(x2 - x1);
    int i = 1;
    while (i <= stopLoop) {
      possibleUncheckMoves.emplace_back(string(1, char(x1 + 96) + i1) + to_string(y1 + i2));
      i1 += i1;
      i2 += i2;
      i++;
    }
  } else {
    // If the checkin piece is not on any vertical, horizontal or diagonal, then the checking piece
    //   must be a knight. Therefore, the only way to stop the check is by taking the knight
    possibleUncheckMoves.emplace_back(checkCoord);
  }
  return possibleUncheckMoves;
} 

void Board::findAllValidMoves(bool firstTurn) { 
  vector<string> validWhiteMoves;
  vector<string> validWhiteKingMoves;
  vector<string> validBlackMoves; 
  vector<string> validBlackKingMoves;
  vector<string> validCheckMoves;
  // Determines which colour's turn it is
  ColourEnum turnColour;
  cout << isWhiteTurn << endl;
  if (isWhiteTurn) {
    turnColour = ColourEnum::White;
  } else {
    turnColour = ColourEnum::Black;
  }
  // Sees if the player's king is in check or not
  string checkStatus = isChecked(isWhiteTurn);
  bool isChecked = checkStatus != "";
  if (isChecked) {
    // Gets the coordinates to stop the check
    validCheckMoves =  possibleUncheckMoves(checkStatus, isWhiteTurn);
  }
  // Iterates through every square on the board
  for (char col = 'a'; col < cols + 97; ++col) {
    for (int row = 1; row <= rows; ++row) {
      // Gets the coordinate of the square
      const string coord = string(1, col) + to_string(row);
      // Checks if there's a piece on the square
      Piece *piece = squares[coord]->getPiece();
      // cout << coord << endl;
      if (piece != nullptr) {
        bool pieceIsWhite = piece->getColour() == ColourEnum::White;
        const PieceEnum pieceEnum = piece->getPieceType();
        // Gets all the potential moves of the piece at coord
        vector<vector<string>> allMoves = piece->getValidMoves(coord, cols, rows, firstTurn);
        // Filters out all the non-valid moves
        if (isChecked) {
          flattenCheckedMoves(coord, pieceEnum,allMoves, validCheckMoves, 
                              piece->getColour(), validWhiteMoves, validBlackMoves);
        } else {
          flattenMoves(coord, pieceEnum, allMoves, piece->getColour(), validWhiteMoves, validBlackMoves);
        }
        if (pieceEnum == PieceEnum::K || pieceEnum == PieceEnum::k ) {
          // Iterates through all the possible moves
          for (auto moves : allMoves) {
            for (auto move : moves) {
              Square *square = squares[move].get();
              if (square != nullptr) {
                Piece *othPiece = square->getPiece();
                // Checks if the square is empty
                if (othPiece != nullptr) {
                  // Checks if the piece at the square is takeable
                  if (othPiece->getColour() != piece->getColour()) {
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
  }
  // for (auto move : validWhiteKingMoves) {
  //   // cout << move << endl;
  // }
  // Filters out any moves that will put their own king in check
  validWhiteKingMoves = filterKingMoves(validWhiteKingMoves, validBlackMoves);
  validBlackKingMoves = filterKingMoves(validBlackKingMoves, validWhiteMoves);
  validWhiteMoves.insert(validWhiteMoves.end(), validWhiteKingMoves.begin(), validWhiteKingMoves.end());
  validBlackMoves.insert(validBlackMoves.end(), validBlackKingMoves.begin(), validBlackKingMoves.end());

  allValidMoves["white"] = validWhiteMoves;
  // for (auto move : allValidMoves["black"]) {
  //   cout << move << endl;
  // }
  allValidMoves["black"] = validBlackMoves;
  // for (auto move : allValidMoves["black"]) {
  //   cout << move << endl;
  // }
}
