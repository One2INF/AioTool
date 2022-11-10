#pragma once

#include <QSerialPort>
#include <QString>
#include <QThread>

#include "ymodem.h"
#include "serialportthread.h"


class transmitter : public QThread
{
  Q_OBJECT

public:
  transmitter(serialportthread *_SerialPort, QObject *parent = nullptr);
  ~transmitter(void);
  void run(void);
  static size_t Read_Block(uint8_t *data, size_t size, uint32_t timeout);
  static size_t Write(const uint8_t *data, size_t size);
  static YMODEM_STATUS_EN receive_data_handler(size_t packet_offset, const uint8_t *data, size_t size);

  void Send(const QString file_name, const quint32 send_times);

signals:
  void UpdateText(QString text);
  void signalWriteData(const char *data, qint64 len);

private:
  QString FileName;
  FILE_INFO_ST FileInfo;
  YMODEM_HANDLER Ymodem;
  bool QuitFlag = false;
  quint32 SendTimes;

  static serialportthread *SerialportHandle;

  void stop(void);
};
