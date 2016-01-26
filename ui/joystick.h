#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <windows.h>
#include <XInput.h>

class Joystick
{
public:
  Joystick();
  ~Joystick();
private:
  XINPUT_STATE    m_controllerState;
  int             m_controllerNum;
};

#endif // JOYSTICK_H
