#include <iostream>
#include <memory>
#include <map>

#include "game.h"
using namespace std;

int main(int argc, char *argv[]) {
  map<string, Player> stringToPlayer = {
    {"human", Human},
    {"computer1", AI1},
    {"computer2", AI2},
    {"computer3", AI3},
  };

  string cmd;
  cin >> cmd;
  if (cmd == "game") {
    string str_p1;
    string str_p2;
    cin >> str_p1 >> str_p2;
    Player p1 = stringToPlayer[str_p1];
    Player p2 = stringToPlayer[str_p2];

    // exit program if players are not recognized
    if (!p1 || !p2) {
      cout << "Invalid player commands" << endl;
      return 1;
    }

    auto game = make_unique<Game>(p1, p2);
    game->start();
  } else if (cmd == "setup") {
    auto game = make_unique<Game>();
    game->setup();
  }
}