/* xbox 360 controller class */

#include "xbox360ctlr.h"
#include <stdio.h>
#include <string.h>
#include <linux/joystick.h>

bool Xbox360Ctlr::Open(char *path) {
  if (!this->Open(path))
    return false;

  if (ioctl(jd_fd_, JSIOCGNAME(sizeof(device_name_)), device_name_) < 0) {
    strncpy(device_name_, "Unknown", sizeof(device_name_));
    return false;
  }

  if (ioctl(jd_fd_, JSIOCGAXES, &num_axes_) < 0) {
    return false;
  }

  if (ioctl(jd_fd_, JSIOCGBUTTONS, &num_buttons_) < 0) {
    return false;
  }

  return true;
}
