#ifndef WIFI_H
#define WIFI_H

#include <QObject>
#include <QStringList>

class Wifi : public QObject
{
  Q_OBJECT
public:
  Wifi();
  ~Wifi();
  bool Init();
  void Deinit();
signals:
  void SignalListPopulate(QStringList);

public slots:
  void Scan();
  void Connect();
};

#endif // WIFI_H
