#ifndef _GRAPHICS_DISPLAY_H_
#define _GRAPHICS_DISPLAY_H_
#include "observer.h"
#include "pieceEnum.cc"
#include "window.h"

class Board;

class GraphicsDisplay : public Observer {
  private:
    Board *subject;
    int cols;
    int rows;
    std::vector<std::vector<PieceEnum>> display; // first col then row
    Xwindow *window;
  public:
    GraphicsDisplay(Board *subject, int cols, int rows);
    void notify() override;
    void render() override;
    ~GraphicsDisplay();
};

#endif
