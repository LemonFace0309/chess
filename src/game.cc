#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "game.h"
#include "board.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
#include "colourEnum.cc"
using namespace std;

void Game::setDefaults() {
  board = make_unique<Board>(8, 8); 
  isWhiteTurn = true;
  stringToColourEnum = {
    {
      {"white", ColourEnum::White},
      {"black", ColourEnum::Black}
    }
  };
}

Game::Game() {
  setDefaults();
  players[0] = Player::Human;
  players[1] = Player::Human;
}

Game::Game(Player p1, Player p2) {
  setDefaults();
  players[0] = p1;
  players[1] = p2;
}

void Game::start() {
  // setting white pieces
  board->setSquare(PieceEnum::R, true, "a1", true);
  board->setSquare(PieceEnum::N, true, "b1", true);
  board->setSquare(PieceEnum::B, true, "c1", true);
  board->setSquare(PieceEnum::Q, true, "d1", true);
  board->setSquare(PieceEnum::K, true, "e1", true);
  board->setSquare(PieceEnum::B, true, "f1", true);
  board->setSquare(PieceEnum::N, true, "g1", true);
  board->setSquare(PieceEnum::R, true, "h1", true);
  for (int i = 0; i < 8; ++i) {
    char c = 97 + i;
    board->setSquare(PieceEnum::P, true, string(1, c) + "2", true);
  }

  // setting black pieces
  for (int i = 0; i < 8; ++i) {
    char c = 97 + i;
    board->setSquare(PieceEnum::P, false, string(1, c) + "7", true);
  }
  board->setSquare(PieceEnum::R, false, "a8", true);
  board->setSquare(PieceEnum::N, false, "b8", true);
  board->setSquare(PieceEnum::B, false, "c8", true);
  board->setSquare(PieceEnum::Q, false, "d8", true);
  board->setSquare(PieceEnum::K, false, "e8", true);
  board->setSquare(PieceEnum::B, false, "f8", true);
  board->setSquare(PieceEnum::N, false, "g8", true);
  board->setSquare(PieceEnum::R, false, "h8", true);

  // rendering the game
  board->finishTurn();

  // beginning game
  begin();
}

void Game::begin() {
  ColourEnum c = isWhiteTurn ? ColourEnum::White :  ColourEnum::Black;

  while (true) {
    string cmd;
    cin >> cmd;

    if (cmd == "move") {
      bool validMove = false;
      string coord1, coord2;

      // get all pseudo moves
      while (!validMove) {
        cin >> coord1 >> coord2;

        // checks for valid move
      }
      // check for checks
      // check for checkmate
      board->finishTurn(); // renders the board
    } else if (cmd == "resign") {
      // do something
    }
    changeTurn(c);
    c = isWhiteTurn ? ColourEnum::Black :  ColourEnum::White; // changes colour for next turn
  }
}

class InvalidPiece {

};

class InvalidOperation {

};

class InvalidColour {

};

std::istream & operator>>( std::istream & in, PieceEnum & p ) {
  char s;
  in >> s;
  switch (s) {
    case 'K':
      p = PieceEnum::K;
      break;
    case 'Q':
      p = PieceEnum::Q;
      break;
    case 'B':
      p = PieceEnum::B;
      break;
    case 'R':
      p = PieceEnum::R;
      break;
    case 'N':
      p = PieceEnum::N;
      break;
    case 'P':
      p = PieceEnum::P;
      break;
    default:
      throw InvalidPiece();
  }
  return in;
}

void Game::setup() {
  string cmd;
  while (true) {
    cin >> cmd;
    try {
      if (cmd == "+") {
        PieceEnum p;
        string loc;
        cin >> p >> loc;

        board->setSquare(p, isWhiteTurn, loc);
        board->finishTurn();
      } else if (cmd == "-") {
        string loc;
        cin >> loc;
        board->setSquare(PieceEnum::NONE, isWhiteTurn, loc);
        board->finishTurn();
      } else if (cmd == "=") {
        string colour;
        cin >> colour;

        ColourEnum c = stringToColourEnum[colour];

        if (stringToColourEnum.find(colour) == stringToColourEnum.end()) {
          throw InvalidColour();
        }

        changeTurn(c);
      } else if (cmd == "done") {
        begin();
        break;
      } else {
        throw InvalidOperation();
      }
    } catch (...) {
      cout << "Invalid response!" << endl;
    }
  }

  if (cmd == "done") start();
}

void Game::changeTurn(ColourEnum c) {
  try {

    if (c == ColourEnum::White) {
      isWhiteTurn = true;
    } else if (c == ColourEnum::Black) {
      isWhiteTurn = false;
    }
  } catch(...) {
    cout << "Invalid Colour!" << endl; 
  }
}