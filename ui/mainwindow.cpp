#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wifi.h"
#include "visualiser.h"
#include "joystick.h"

#include <QObject>
#include <QDebug>
#include <QStringListModel>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainWindow),
  m_wifi(new Wifi),
  m_wifiListModel(new QStringListModel()),
  m_joystick(new Joystick())
{
  m_ui->setupUi(this);

  QObject::connect(
    m_ui->scanPushButton,
    SIGNAL(clicked(bool)),
    m_wifi,
    SLOT(Scan())
  );
  QObject::connect(
    m_ui->connectPushButton,
    SIGNAL(clicked(bool)),
    m_wifi,
    SLOT(Connect())
  );
  QObject::connect(
    m_wifi,
    SIGNAL(SignalListPopulate(QStringList)),
    this,
    SLOT(PopulateWifiList(QStringList))
  );
  QObject::connect(
    m_ui->Visualizer,
    SIGNAL(LogUartConsole(QString)),
    this,
    SLOT(appendToUartConsole(QString))
  );
}

MainWindow::~MainWindow()
{
  delete m_ui;
  delete m_wifi;
  delete m_wifiListModel;
}

void MainWindow::PopulateWifiList(QStringList list) {
  m_wifiListModel->setStringList(list);
  if (m_ui->wifiList != NULL) {
    m_ui->wifiList->setModel(m_wifiListModel);
  }
}

void MainWindow::appendToUartConsole(QString str) {
  m_ui->UartWifiConsole->append(str);
}

void MainWindow::on_Load3DModelButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
    this,
    tr("Open Image"),
    "",
    tr("Image Files (*.obj)")
  );
  if (!m_ui->Visualizer->LoadModel(fileName))
    exit(1);
}
