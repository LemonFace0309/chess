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

map<string, ColourEnum> stringToColourEnum = {
  {"white", ColourEnum::White},
  {"black", ColourEnum::Black}
};

map<ColourEnum, string> colourEnumToString = {
  {ColourEnum::White, "White"},
  {ColourEnum::Black, "Black"}
};

void Game::setDefaults() {
  board = make_unique<Board>(8, 8); 
  isWhiteTurn = true;
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

void Game::addPoints(ColourEnum winner, ColourEnum loser, bool stalemate) {
  int winnerPoints = stalemate ? 0.5 : 1;
  int loserPoints = stalemate ? 0.5 : 0;

  score[winner] = winnerPoints;
  score[loser] = winnerPoints;
}

void Game::printScore() {
  cout << "Final Score:" << endl;
  for (auto &p : stringToColourEnum) {
    string colour = p.first;
    colour[0] = toupper(colour[0]);
    cout << colour << ": " << score[p.second] << endl;
  }
}

void Game::setWinner(ColourEnum winner, ColourEnum loser, bool stalemate) {
  cout << "Congratulations " << colourEnumToString[winner] << ", you won! 🥰" << endl;
  addPoints(winner, loser, stalemate);
  printScore();
  board->reset();
  cout << endl;
  cout << "Now starting a new game" << endl;
  cout << endl;
  board->render();
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
  board->finishTurn(true);

  // beginning game
  begin();
}

void Game::begin() {
  ColourEnum colour = isWhiteTurn ? ColourEnum::White :  ColourEnum::Black;
  board->setTurn(isWhiteTurn);
  cout << "Let's begin our game! We'll start with " << colourEnumToString[colour] << endl;

  while (true) {
    ColourEnum oppColour = colour == ColourEnum::White ? ColourEnum::Black :  ColourEnum::White;
    string cmd;
    cin >> cmd;
    if (cin.eof()) {
      printScore();
      break;
    }

    if (cmd == "move") {
      bool validMove = false;
      string coord1, coord2;

      while (!validMove) {
        cin >> coord1 >> coord2;
        if (cin.eof()) {
          printScore();
          break;
        }

        // checks for valid move on square
        validMove = board->isValidMove(coord1, coord2, false);
        if (!validMove) {
          cout << "Invalid Move!" << endl;
        }
      }
      board->move(coord1, coord2);
      board->finishTurn(false); // renders the board and calculates valid moves

      if (board->getWinner() != ColourEnum::NO_COLOUR) { // checkmate!
        setWinner(board->getWinner(), board->getLoser());
      } else { // outputting message for next user
        cout << colourEnumToString[colour] << " move over! " << colourEnumToString[oppColour] << " turn now 😁" << endl;
        if (board->isPlayerChecked(oppColour) != "") {
          cout << "Check! 😱😨😩" << colourEnumToString[oppColour] << " you king is threatened!";
        }
      }

    } else if (cmd == "resign") {
      board->setWinner(oppColour);
      board->setLoser(colour);
      setWinner(oppColour, colour);
    } else {
      // reads till end of line to prevent Invalid response from being printed out multiple times
      while (cin.peek() != '\n') {
        cin.ignore();
      }
      cout << "Invalid Command!" << endl;
    }
    changeTurn(colour);
    colour = oppColour; // changes colour for next turn
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
    if (cin.eof()) break;
  
    try {
      if (cmd == "+") {
        PieceEnum p;
        string loc;
        cin >> p >> loc;

        board->setSquare(p, isWhiteTurn, loc);
        board->render();
      } else if (cmd == "-") {
        string loc;
        cin >> loc;
        board->setSquare(PieceEnum::NONE, isWhiteTurn, loc);
        board->render();
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
      // reads till end of line to prevent Invalid response from being printed out multiple times
      while (cin.peek() != '\n') {
        cin.ignore();
      }
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

Game::~Game() {};
