#ifndef _TEXTDISPLAY_H_
#define _TEXTDISPLAY_H_
#include <iostream>
#include <vector>

#include "display.h"
#include "pieceEnum.cc"

class Square;

class TextDisplay : public Display {
  private:
    int cols;
    int rows;
    std::vector<std::vector<PieceEnum>> display; // first col then row
  public:
    TextDisplay(int cols, int rows);
    void notify(Square *);
    void render();
    ~TextDisplay();
};

#endif