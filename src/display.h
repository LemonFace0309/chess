#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <iostream>

#include "observer.h"

class Display : public Observer {
  public:
    void render();
    virtual ~Display() = 0;
};

#endif
