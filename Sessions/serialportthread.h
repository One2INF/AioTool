#ifndef SERIALPORTTHREAD_H
#define SERIALPORTTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QTimer>


typedef struct
{
  QString portname;
  qint32 baudrate;
  QSerialPort::StopBits stopbits;
  QSerialPort::Parity parity;
  QSerialPort::FlowControl flowcontrol;
}SERAILPORT_CONFIG_ST;


class serialportthread : public QThread
{
  Q_OBJECT

public:
  explicit serialportthread(QObject *parent = nullptr);
  void run(void);
  bool open(SERAILPORT_CONFIG_ST *config);
  void close(void);
  bool isOpen(void);

  size_t read(char* data, size_t size);
  size_t read_block(char* data, size_t size, uint32_t timeout);
  size_t write(const char* data, size_t size);

private:
  QSerialPort SerialPort;
  QList<QByteArray> listReadData;
  QList<QByteArray> listWriteData;

  QByteArray tempReadData;
  QTimer ReadTimer;
  bool Running = false;

  void init(void);

signals:
  void signalReadyRead(QByteArray data);

private slots:
  void slotReadyRead(void);
  void slotSerialportIdle(void);
};

#endif // SERIALPORTTHREAD_H
