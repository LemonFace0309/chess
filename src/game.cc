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
  board->setSquare(PieceEnum::R, true, "a1");
  board->setSquare(PieceEnum::N, true, "b1");
  board->setSquare(PieceEnum::B, true, "c1");
  board->setSquare(PieceEnum::Q, true, "d1");
  board->setSquare(PieceEnum::K, true, "e1");
  board->setSquare(PieceEnum::B, true, "f1");
  board->setSquare(PieceEnum::N, true, "g1");
  board->setSquare(PieceEnum::R, true, "h1");
  for (int i = 0; i < 8; ++i) {
    char c = 97 + i;
    board->setSquare(PieceEnum::P, true, string(1, c) + "2");
  }

  // setting black pieces
  for (int i = 0; i < 8; ++i) {
    char c = 97 + i;
    board->setSquare(PieceEnum::P, false, string(1, c) + "7");
  }
  board->setSquare(PieceEnum::R, false, "a8");
  board->setSquare(PieceEnum::N, false, "b8");
  board->setSquare(PieceEnum::B, false, "c8");
  board->setSquare(PieceEnum::Q, false, "d8");
  board->setSquare(PieceEnum::K, false, "e8");
  board->setSquare(PieceEnum::B, false, "f8");
  board->setSquare(PieceEnum::N, false, "g8");
  board->setSquare(PieceEnum::R, false, "h8");

  // rendering the game
  board->finishTurn();
}

class InvalidPiece {

};

class InvalidOperation {

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
        changeTurn(colour);
      } else if (cmd == "done") {
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

class InvalidColour {

};

void Game::changeTurn(string colour) {
  try {
    ColourEnum c = stringToColourEnum[colour];

    if (c == ColourEnum::White) {
      isWhiteTurn = true;
    } else if (c == ColourEnum::Black) {
      isWhiteTurn = false;
    } else {
      throw InvalidColour();
    }
  } catch(...) {
    cout << "Invalid Colour!" << endl; 
  }
}