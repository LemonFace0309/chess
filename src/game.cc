#include <iostream>
#include <memory>
#include <map>

#include "game.h"
#include "board.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
#include "colourEnum.cc"
using namespace std;

Game::Game() : board{make_unique<Board>(8, 8)}, isWhiteTurn{true},
  stringToColourEnum{
    {
      {"white", ColourEnum::White},
      {"black", ColourEnum::Black}
    }
  } {
  players[0] = Player::Human;
  players[1] = Player::Human;
}

Game::Game(Player p1, Player p2) {
  players[0] = p1;
  players[1] = p2;
}

void Game::start() {

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
    try {
      if (cmd == "+") {
        PieceEnum p;
        string loc;
        cin >> p >> loc;
        board->setSquare(p, isWhiteTurn, loc);
      } else if (cmd == "-") {
        string loc;
        cin >> loc;
        // do something
      } else if (cmd == "done") {
        break;
      } else {
        throw InvalidOperation();
      }
    } catch (...) {
      cout << "Invalid response!" << endl;
    }

    cin >> cmd;
  }

  if (cmd == "done") start();
}

class InvalidColour {

};

void Game::changeTurn(std::string colour) {
  try {
    ColourEnum c = stringToColourEnum[colour];
    if (!c) throw InvalidColour();

    if (c == White) {
      isWhiteTurn = true;
    } else if (c == Black) {
      isWhiteTurn = false;
    }
  } catch(...) {
    cout << "Invalid Colour!" << endl; 
  }
}