#include "joystick.h"
#include <fcntl.h>
#include <unistd.h>

using namespace std;

Joystick::Joystick()
  : jd_fd_(-1),
  num_axes_(0),
  num_buttons_(0)
{}

Joystick::~Joystick() {
  if (jd_fd_ != -1)
    close(jd_fd_);
}

bool Joystick::Open(char *path) {
  jd_fd_ = open(path, O_RDONLY);
  if (jd_fd_ > -1)
    return true;
  else
    return false;
}
