#include "serialportthread.h"

#include <QSerialPort>
#include <QDebug>


#define SERIALPORT_RECEIVE_TIMREOUT    10


serialportthread::serialportthread(QObject *parent)
  : QThread(parent)
{
  connect(&SerialPort, &QSerialPort::readyRead,
          this, &serialportthread::slotReadyRead);
  connect(&ReadTimer, &QTimer::timeout,
          this, &serialportthread::slotSerialportIdle);
}

size_t serialportthread::read(char* data, size_t size)
{
  if(listReadData.isEmpty())
    return 0;

  QByteArray ReadData = listReadData.front();

  if(size > ReadData.size())
    size = ReadData.size();

  memcpy(data, ReadData.constData(), size);
  listReadData.removeFirst();

  return size;
}

size_t serialportthread::read_block(char* data, size_t size, uint32_t timeout)
{
  while(timeout--)
  {
    if(listReadData.length())
    {
      QByteArray ReadData = listReadData.front();

      if(size > ReadData.size())
        size = ReadData.size();

      memcpy(data, ReadData.constData(), size);
      listReadData.removeFirst();
      return size;
    }
    msleep(1);
  }
  return 0;
}

size_t serialportthread::write(const char* data, size_t size)
{
  listWriteData.append(QByteArray(data, size));
  return size;
}

bool serialportthread::open(SERAILPORT_CONFIG_ST *config)
{
  SerialPort.setPortName(config->portname);
  SerialPort.setBaudRate(config->baudrate);
  SerialPort.setStopBits(config->stopbits);
  SerialPort.setParity(config->parity);
  SerialPort.setFlowControl(config->flowcontrol);

  if(SerialPort.open(QIODevice::ReadWrite))
  {
    SerialPort.moveToThread(this);
    init();
    start();
    return true;
  }

  return false;
}

bool serialportthread::isOpen(void)
{
  return SerialPort.isOpen();
}

void serialportthread::init(void)
{
  Running = true;
  listReadData.clear();
  listWriteData.clear();
}

void serialportthread::close(void)
{
  Running = false;
  quit();
}

void serialportthread::run(void)
{
  while(Running)
  {
    while(listWriteData.length())
    {
      QByteArray data = listWriteData.first();
      SerialPort.write(data.constData(), data.size());
      listWriteData.removeFirst();

      SerialPort.waitForBytesWritten();
    }
  }

  SerialPort.close();
}

void serialportthread::slotReadyRead(void)
{
  tempReadData += SerialPort.readAll();
  ReadTimer.start(SERIALPORT_RECEIVE_TIMREOUT);
}

void serialportthread::slotSerialportIdle(void)
{
  listReadData.append(tempReadData);

  emit signalReadyRead(listReadData.last());

  tempReadData.clear();
  ReadTimer.stop();
}
