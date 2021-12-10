#include <iostream>
#include "game.h"
#include "playerEnum.h"
using namespace std;

Game::Game() {
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

enum Piece {
  K,
  Q,
  B,
  R,
  N,
  P
};

std::istream & operator>>( std::istream & in, Piece & p ) {
   char s;
   in >> s;
  switch (s) {
    case 'K':
      p = K;
      break;
    case 'Q':
      p = Q;
      break;
    case 'B':
      p = B;
      break;
    case 'R':
      p = R;
      break;
    case 'N':
      p = N;
      break;
    case 'P':
      p = P;
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
        Piece p;
        string loc;
        cin >> p >> loc;
        // do something
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