#pragma once

#include <QSerialPort>
#include <QString>
#include <QThread>


typedef struct
{
  QString portname;
  qint32 baudrate;
}SERIALPORT_INFO_ST;

class serialport : public QThread
{
  Q_OBJECT

public:
  serialport(QObject *parent = nullptr);
  ~serialport(void);
  void init(SERIALPORT_INFO_ST *serialport_info);
  void run(void);
  void stop(void);

private:
  QSerialPort SerialPort;
};
