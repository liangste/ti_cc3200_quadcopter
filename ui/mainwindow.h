#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStringList>
#include <QStringListModel>

#include "wifi.h"
#include "visualiser.h"
#include "joystick.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow      *m_ui;
  Wifi                *m_wifi;
  QStringListModel    *m_wifiListModel;
  Visualiser          *m_visualiser;
  Joystick            *m_joystick;

public slots:
  void PopulateWifiList(QStringList);
  void appendToUartConsole(QString);
private slots:
  void on_Load3DModelButton_clicked();
};

#endif // MAINWINDOW_H
