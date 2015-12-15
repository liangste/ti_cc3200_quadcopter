#include <QApplication>
#include <QPushButton>
#include <syslog.h>
#include "joystick.h"

using namespace std;

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QWidget window;
  window.show();

  return app.exec();
}
