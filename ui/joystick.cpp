#include "joystick.h"

Joystick::Joystick() {
  m_controllerNum = 0;
  XInputGetState(m_controllerNum, &m_controllerState);
}

Joystick::~Joystick() {
}
