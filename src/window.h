#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[10];
  const int squareSize = 20; // The size of a square on the chess board

 public:
  Xwindow();  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.
  Xwindow(const Xwindow&) = delete;
  Xwindow &operator=(const Xwindow&) = delete;

  void clearRow(int y);

  // Available colours.
  enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown};

  // Draws a square
  void drawSquare(int x, int y, int colour=Black);

  // Draws a string
  void drawString(int x, int y, std::string msg, int colour);

};

#endif
