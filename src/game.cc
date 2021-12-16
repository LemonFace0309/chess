#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <sstream>

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

void Game::printMessage(string msg) {
  cout << msg << endl;
  board->displayGraphicText(msg);
}

void Game::addPoints(ColourEnum winner, ColourEnum loser, bool stalemate) {
  int winnerPoints = stalemate ? 0.5 : 1;
  int loserPoints = stalemate ? 0.5 : 0;

  score[winner] = winnerPoints;
  score[loser] = winnerPoints;
}

void Game::printScore() {
  string output = "Final Score:";
  cout << "Final Score:" << endl;
  for (auto &p : stringToColourEnum) {
    string colour = p.first;
    colour[0] = toupper(colour[0]);
    output += colour + ": " + to_string(score[p.second]) +", ";
    cout << colour << ": " << to_string(score[p.second]) << endl;
  }
  board->displayGraphicText(output);
}

void Game::setWinner(ColourEnum winner, ColourEnum loser, bool stalemate) {  
  ostringstream ss;
  ss << "Congratulations " << colourEnumToString[winner] << ", you won! :)";
  printMessage(ss.str());
  addPoints(winner, loser, stalemate);
  printScore();
  board->reset();
  cout << endl;
  string output = "Now starting a new game";
  cout << output << endl;
  cout << endl;
  board->displayGraphicText(output);
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
  printMessage("Let's begin our game! We'll start with " + colourEnumToString[colour]);
  int turns = 1;

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
        validMove = board->isValidMove(coord1, coord2, turns <= 2);
        if (!validMove) {
           printMessage("Invalid Move!");
        }
      }
      board->move(coord1, coord2, turns <= 2);
      board->finishTurn(turns <= 2); // renders the board and calculates valid moves

      if (board->getWinner() != ColourEnum::NO_COLOUR) { // checkmate!
        setWinner(board->getWinner(), board->getLoser());
        turns = 0;
      } else { // outputting message for next user
        ostringstream ss;
        ss << colourEnumToString[colour] << " move over! " << colourEnumToString[oppColour] << " turn now :D";
        printMessage(ss.str());
        cout <<  << endl;
        if (board->isPlayerChecked(oppColour) != "") {
          ostringstream ss2;
          ss2 << "Check! :O D: :X" << colourEnumToString[oppColour] << " you king is threatened!";
          printMessage(ss2.str());
        }
      }

      ++turns;
    } else if (cmd == "resign") {
      board->setWinner(oppColour);
      board->setLoser(colour);
      setWinner(oppColour, colour);
      turns = 1;
    } else {
      // reads till end of line to prevent Invalid response from being printed out multiple times
      while (cin.peek() != '\n') {
        cin.ignore();
      }
      printMessage("Invalid Command!");
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
      printMessage("Invalid response!");
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
    printMessage("Invalid Colour!");
  }
}

Game::~Game() {};
