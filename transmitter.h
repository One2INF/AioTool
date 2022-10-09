#pragma once

#include <QSerialPort>
#include <QString>
#include <QThread>

#include "ymodem.h"


typedef struct
{
  QString portname;
  qint32 baudrate;
}SERIALPORT_INFO_ST;


class transmitter : public QThread
{
  Q_OBJECT

public:
  transmitter(QObject *parent = nullptr);
  ~transmitter(void);
  void init(SERIALPORT_INFO_ST *serialport_info);
  void run(void);
  static size_t Read_Block(uint8_t *data, size_t size, uint32_t timeout);
  static size_t Write(uint8_t *data, size_t size);
  static YMODEM_STATUS_EN receive_data_handler(uint8_t *data, size_t size);
  static QSerialPort SerialPort;

private:
  SERIALPORT_INFO_ST SerialportInfo;
  YMODEM_HANDLER Ymodem;
  bool QuitFlag = false;

  void stop(void);
};
