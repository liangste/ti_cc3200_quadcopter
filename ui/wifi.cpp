#include "wifi.h"
#include <QObject>
#include <QStringList>
#include <QStringListModel>
#include <QApplication>
#include <QMainWindow>

Wifi::Wifi() {
}

Wifi::~Wifi() {
}

bool Wifi::Init() {
  return true;
}

void Wifi::Deinit() {
}

void Wifi::Scan() {
  QStringList list;
  SignalListPopulate(list);
}

void Wifi::Connect() {
}
