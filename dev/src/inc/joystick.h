#ifndef JOYSTICK_H
#define JOYSTICK_H

class Joystick {
public:
  Joystick();
  ~Joystick();

  // Open joystick device pointed to by path
  bool Open(char *path);

  // Register joystick device to update values per interval
  //virtual bool Register(int interval);
protected:
  int jd_fd_;
  char num_axes_;
  char num_buttons_;
  char device_name_[64];
};

#endif
