#include "serialport.h"

#include <QtSerialPort/QSerialPort>
#include <QDebug>


serialport::serialport(QObject *parent) : QThread(parent)
{

}

serialport::~serialport(void)
{
  this->stop();
}

void serialport::init(SERIALPORT_INFO_ST *serialport_info)
{
  SerialPort.setPortName(serialport_info->portname);
  SerialPort.setBaudRate(serialport_info->baudrate);
  SerialPort.setStopBits(QSerialPort::OneStop);
  SerialPort.setDataBits(QSerialPort::Data8);
  SerialPort.setParity(QSerialPort::NoParity);
  SerialPort.setFlowControl(QSerialPort::NoFlowControl);
  if(!SerialPort.open(QIODevice::ReadWrite))
    qDebug() << "open " + SerialPort.portName() + " failed!";
}

void serialport::run(void)
{
  while(1)
  {
    if(!SerialPort.isOpen())
    {
      msleep(10);
      continue;
    }

    static int i = 0;
    QString SendData = ((QString)"Hello %1").arg(++i);
    SerialPort.write(SendData.toUtf8());

    QByteArray ReadData = SerialPort.readAll();
    qDebug() << "read: " << ReadData;

    msleep(1000);
  }
}

void serialport::stop(void)
{
  if(SerialPort.isOpen())
    SerialPort.close();

  this->quit();
  this->wait();
}
