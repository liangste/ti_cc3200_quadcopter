#ifndef XBOX360CTLR_H
#define XBOX360CTLR_H

#include "joystick.h"

class Xbox360Ctlr : public Joystick {
public:
  Xbox360Ctlr();
  ~Xbox360Ctlr();
  bool Open(char *path);
};

#endif
