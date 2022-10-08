#include "serialport.h"

#include <QtSerialPort/QSerialPort>
#include <QDebug>

bool OpenSerialport(QSerialPort &serial_port, SERIALPORT_INFO_ST *serialport_info);


serialport::serialport(QObject *parent) : QThread(parent)
{

}

serialport::~serialport(void)
{
  stop();
  wait();
}

void serialport::init(SERIALPORT_INFO_ST *serialport_info)
{
  SerialportInfo = *serialport_info;
}

void serialport::run(void)
{
  QSerialPort SerialPort;
  if(!OpenSerialport(SerialPort, &SerialportInfo))
  {
    qDebug() << "open " + SerialPort.portName() + " failed!";
    return;
  }

  while(!QuitFlag)
  {
    if(!SerialPort.isOpen())
    {
      msleep(10);
      continue;
    }

    static int i = 0;
    QString SendData = ((QString)"Hello %1").arg(i++);
    SerialPort.write(SendData.toUtf8());
    SerialPort.waitForBytesWritten(10);

    QByteArray ReadData = SerialPort.readAll();
    while (SerialPort.waitForReadyRead(10))
        ReadData += SerialPort.readAll();
    qDebug() << "read: " << ReadData;

    msleep(1000);
  }

  if(SerialPort.isOpen())
  {
    SerialPort.close();
  }
}

void serialport::stop(void)
{
  QuitFlag = true;
}

bool OpenSerialport(QSerialPort &serial_port, SERIALPORT_INFO_ST *serialport_info)
{
  serial_port.setPortName(serialport_info->portname);
  serial_port.setBaudRate(serialport_info->baudrate);
  serial_port.setStopBits(QSerialPort::OneStop);
  serial_port.setDataBits(QSerialPort::Data8);
  serial_port.setParity(QSerialPort::NoParity);
  serial_port.setFlowControl(QSerialPort::NoFlowControl);

  return serial_port.open(QIODevice::ReadWrite);
}
