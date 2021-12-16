#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>

#include "window.h"
#include "board.h"
#include "textdisplay.h"
#include "graphicsDisplay.h"
#include "square.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"
#include "pawn.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
#include "colourEnum.cc"
using namespace std;

void Board::setDefaults() {
  whiteKingCoord = "";
  blackKingCoord = "";
  isWhiteTurn = true;
  winner = ColourEnum::NO_COLOUR;
  loser = ColourEnum::NO_COLOUR;
  lastAction = "";
  allValidMoves.clear();
}

Board::Board(const int rows, const int cols) : rows{rows}, cols{cols} {
  setDefaults();
  shared_ptr<TextDisplay> td = make_shared<TextDisplay>(this, cols, rows);
  attach(td);
  window = make_shared<Xwindow>();
  shared_ptr<GraphicsDisplay> gd = make_shared<GraphicsDisplay>(this, cols, rows, window);
  attach(gd);

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

  if (p == PieceEnum::k && !isWhiteTurn) {
    blackKingCoord = coord;
  } else if (p == PieceEnum::K && isWhiteTurn) {
    whiteKingCoord = coord;
  }
  squares[coord]->setPiece(p, isWhiteTurn, firstTurn);
  lastAction = coord;
  notifyObservers();
  return true;
}

void Board::reset() {
  setDefaults();
}

void Board::setTurn(bool isWhiteTurn) {
  isWhiteTurn = isWhiteTurn;
}

ColourEnum Board::getLoser() {
  return loser;
}

void Board::setLoser(ColourEnum c) {
  loser = c;
}

ColourEnum Board::getWinner() {
  return winner;
}

void Board::setWinner(ColourEnum c) {
  winner = c;
}

void Board::render() {
  renderObservers();
}

void Board::finishTurn(bool firstTurn) {
  render();
  findAllValidMoves(firstTurn);
}


vector<string> filterKingMoves(vector<string> validKingMoves, vector<string> validOpponentMoves) {
  for (auto move : validKingMoves) {
    istringstream coords{ move };
    string initCoord;
    string finalCoord;
    coords >> initCoord;
    coords >> finalCoord;
    for (auto oMoves : validOpponentMoves) {
      int i = oMoves.find(finalCoord, 3);
      if (i != -1) {
        validKingMoves.erase(validKingMoves.begin() + i);
      }
    }
  }
  return validKingMoves;
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
        Square *square = squares[move].get();
        if (square != nullptr) {
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
              addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);             
            }
            break;
          } else {
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
                addMove(coord + " " + move, pieceIsWhite, validWhiteMoves, validBlackMoves);             
              }
              break;
            } else {
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
              addMove(coord + " " + coords, pieceIsWhite, validWhiteMoves, validBlackMoves);             
            }
          }
        }
      }
    }
  }
}

void Board::move(string coord1, string coord2) {
  PieceEnum p = squares[coord1]->getPiece()->getPieceType();
  Board::setSquare(p, isWhiteTurn, coord2);
  Board::setSquare(PieceEnum::NONE, isWhiteTurn, coord1);
  isWhiteTurn = !isWhiteTurn;
}

// Checks if the move is valid.
// coord1 is where the piece is coming from
// coord2 is where the piece is coming to
bool Board::isValidMove(string coord1, string coord2, bool firstTurn) {
  string colourTurn;
  ColourEnum colour;

  if (isWhiteTurn) {
    colourTurn = "white";
    colour = ColourEnum::White;
  } else {
    colourTurn = "black";
    colour = ColourEnum::Black;
  }
  string move = coord1 + " " + coord2;

  // Checks if the move is valid (doesn't include being under check)
  if (find(allValidMoves[colourTurn].begin(), allValidMoves[colourTurn].end(), move)
      != allValidMoves[colourTurn].end()) {
    // Keeps track of the old piece at the square in case the move isn't actually valid, ie piece can't be moved
    //   since it's protecting the king.

    PieceEnum capturedPieceEnum;
    Piece *capturedPiece = squares[coord2]->getPiece();
    if (capturedPiece == nullptr) {
      capturedPieceEnum = PieceEnum::NONE;
    } else {
      capturedPieceEnum = capturedPiece->getPieceType();
    }        

    // Temporarily moves the piece
    PieceEnum p = squares[coord1]->getPiece()->getPieceType();
    squares[coord2]->setPiece(p, isWhiteTurn, firstTurn);

    // Checks if the king is under check after the move
    bool isMovable = isPlayerChecked(isWhiteTurn) == "";

    // Undo moves
    squares[coord1]->setPiece(p, isWhiteTurn, firstTurn);
    squares[coord2]->setPiece(capturedPieceEnum, isWhiteTurn, firstTurn);

    return isMovable;
  }
  return false;
}

// Checks if the black or white king is being checked.
// Returns "" if the king isn't being checked, and the coordinate 
//   of the piece that is checking the king if it is
string Board::isPlayerChecked(bool isWhiteTurn) {
  ColourEnum colour;
  // Gets the coordinates of the black or white king
  string coord;

  if (isWhiteTurn) {
    colour = ColourEnum::White; 
    coord = whiteKingCoord;
  } else {
    colour = ColourEnum::Black; 
    coord = blackKingCoord;
  }

  // Gets the coordinates of possible checks by a knight
  Knight knight{colour};
  vector<vector<string>> knightChecks = knight.getValidMoves(coord, cols, rows, false);
  for (auto coords : knightChecks) {
    for (auto kcoord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[kcoord].get();
      if (square != nullptr) {
        // Checks if the piece at the coordinate is an opposing knight
        Piece * piece = square->getPiece();
        if (piece != nullptr && piece->getColour() != colour) {
          if (isWhiteTurn && piece->getPieceType() == PieceEnum::n) {
            return kcoord;
          } else if (!isWhiteTurn && piece->getPieceType() == PieceEnum::N) {
            return kcoord;
          }             
        }   
      }
    }
  }

  // Gets the coordinates of possible checks of the row and col the king is at
  Rook rook{colour};
  vector<vector<string>> vertHorzChecks = rook.getValidMoves(coord, cols, rows, false);
  for (auto coords : vertHorzChecks) {
    for (auto rcoord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[rcoord].get();
      if (square != nullptr) {
        Piece * piece = square->getPiece();
        // Checks if the piece at the coordinate is an opposing rook or queen, since only
        //   rooks and queens can move horizontally/vertically
        if (piece != nullptr) {
          if (piece->getColour() != colour) {
            if (isWhiteTurn) {
              if (piece->getPieceType() == PieceEnum::r || piece->getPieceType() == PieceEnum::q)
                return rcoord;
            } else if (!isWhiteTurn) {
              if (piece->getPieceType() == PieceEnum::R || piece->getPieceType() == PieceEnum::Q)
                return rcoord;
            }
          }          
        }   
      }
    }
  }

  // Gets the coordinates of possible checks of the diagonals the king is at
  Bishop bishop{colour};
  vector<vector<string>> diagonalChecks = bishop.getValidMoves(coord, cols, rows, false);
  for (auto coords : diagonalChecks) {
    for (auto bcoord : coords) {
      // Checks if the square exists on the board
      Square *square = squares[bcoord].get();
      if (square != nullptr) {
        // Checks if the piece at the coordinate is an opposing bishop or queen, since only
        //   bishops and queens can move diagonally
        Piece * piece = square->getPiece();
        if (piece != nullptr) {
          if (piece->getColour() != colour) {
            if (isWhiteTurn) {
              if (piece->getPieceType() == PieceEnum::b || piece->getPieceType() == PieceEnum::q)
                return bcoord;
            } else if (!isWhiteTurn) {
              if (piece->getPieceType() == PieceEnum::B || piece->getPieceType() == PieceEnum::Q)
                return bcoord;
            } 
          } else {
            break;
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
        if (isWhiteTurn) {
          if (piece->getPieceType() == PieceEnum::p)
            return coord;
        } else if (!isWhiteTurn) {
          if (piece->getPieceType() == PieceEnum::P)
            return coord;
        }    
      }
    }
  }
  return "";
}

// Returns a vector of the coordinates that can help block/ stop the check
vector<string> Board::possibleUncheckMoves(string checkCoord, bool isWhiteTurn) {
  vector<string> moves;
  string coord;
  if (isWhiteTurn) {
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
      moves.emplace_back(string(1, char(x1 + 96)) + to_string(y1 + i));
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
      moves.emplace_back(string(1, char(x1 + 96) + i) + to_string(y1));
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
      moves.emplace_back(string(1, char(x1 + 96) + i1) + to_string(y1 + i2));
      i1 += i1;
      i2 += i2;
      i++;
    }
  } else {
    // If the checkin piece is not on any vertical, horizontal or diagonal, then the checking piece
    //   must be a knight. Therefore, the only way to stop the check is by taking the knight
    moves.emplace_back(checkCoord);
  }
  return moves;
} 

/**
 * Todo: Calculate pseudo moves (castling + enpassant)
 **/
void Board::findAllValidMoves(bool firstTurn) { 
  vector<string> validWhiteMoves;
  vector<string> validWhiteKingMoves;
  vector<string> validBlackMoves; 
  vector<string> validBlackKingMoves;
  vector<string> validCheckMoves; // moves to escape from checked position
  // Determines which colour's turn it is
  ColourEnum turnColour;
  if (isWhiteTurn) {
    turnColour = ColourEnum::White;
  } else {
    turnColour = ColourEnum::Black;
  }
  // Sees if the player's king is in check or not
  string checkStatus = isPlayerChecked(isWhiteTurn);
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
      bool isFirstTurn = squares[coord]->isFirstTurn();

      if (piece != nullptr) {
        bool pieceIsWhite = piece->getColour() == ColourEnum::White;
        const PieceEnum pieceEnum = piece->getPieceType();
        // Gets all the potential moves of the piece at coord
        vector<vector<string>> allMoves = piece->getValidMoves(coord, cols, rows, isFirstTurn);
        // Filters out all the non-valid moves
        if (isChecked) {
          // like flattenMoves except the only allowed moves are moves that stop the check
          flattenCheckedMoves(coord, pieceEnum,allMoves, validCheckMoves, 
                              piece->getColour(), validWhiteMoves, validBlackMoves);
        } else {
          flattenMoves(coord, pieceEnum, allMoves, piece->getColour(), validWhiteMoves, validBlackMoves);
        }

        if (pieceEnum == PieceEnum::K || pieceEnum == PieceEnum::k ) {
          // Iterates through all the possible moves
          for (auto movesDir : allMoves) {
            for (auto move : movesDir) {
              Square *square = squares[move].get();
              if (square != nullptr) { // checks if square is valid on board
                Piece *othPiece = square->getPiece(); // checks if there is a piece on the square
                
                // Checks if the King can take the square
                if (othPiece == nullptr || othPiece->getColour() != piece->getColour()) {
                  addMove(coord + " " + move, pieceIsWhite, validWhiteKingMoves, validBlackKingMoves);    
                }
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

  // Combining king moves with moves from other pieces
  validWhiteMoves.insert(validWhiteMoves.end(), validWhiteKingMoves.begin(), validWhiteKingMoves.end());
  validBlackMoves.insert(validBlackMoves.end(), validBlackKingMoves.begin(), validBlackKingMoves.end());

  // Check checkmate
  // 1. no valid moves and currently checked
  // 2. valid moves lead to another check and currently checked
  if (isChecked && validWhiteMoves.empty()) {
    setWinner(ColourEnum::Black);
  } else if (isChecked && validBlackMoves.empty()) {
    setWinner(ColourEnum::White);
  } 
  if (isChecked) {
    vector<string> validMoves;
    if (isWhiteTurn) {
      validMoves = validWhiteMoves;
    } else {
      validMoves = validBlackMoves;
    }
    bool isCheckMate = true;
    for (auto move : validMoves) {
      istringstream coords{ move };
      string coord1;
      string coord2;
      coords >> coord1;
      coords >> coord2;
      if (isValidMove(coord1, coord2, firstTurn)) {
        isCheckMate = false;
      }        
    }
    if (isCheckMate) {
      setWinner(turnColour);      
    }
  }

  allValidMoves["white"] = validWhiteMoves;
  allValidMoves["black"] = validBlackMoves;
}

void Board::displayGraphicText(string msg) {
  int x = 0;
  int y = 240;
  window->clearRow(y);
  window->drawString(x, y, msg, Xwindow::Black);
}

Board::~Board() {};
