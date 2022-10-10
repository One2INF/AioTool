#include "transmitter.h"

#include <QtSerialPort>
#include <QDebug>

bool OpenSerialport(QSerialPort &serial_port, SERIALPORT_INFO_ST *serialport_info);


transmitter::transmitter(QObject *parent) : QThread(parent)
{

}

transmitter::~transmitter(void)
{
  stop();
  wait();
}

size_t transmitter::Read_Block(uint8_t *data, size_t size, uint32_t timeout)
{
  QByteArray responseData;
  if(SerialPort.waitForReadyRead(timeout))
  {
    do
    {
      responseData += SerialPort.readAll();
    }
    while(SerialPort.waitForReadyRead(10) || SerialPort.bytesAvailable());

    qDebug() << "read: " << QString::fromUtf8(responseData.toHex());
    memcpy(data, responseData.data(), responseData.size());
  }

  return responseData.size();
}

size_t transmitter::Write(uint8_t *data, size_t size)
{
  QByteArray bytedata = QByteArray((const char*)data, size);
  qDebug() << "write: " << QString::fromUtf8(bytedata.toHex());
  return SerialPort.write((char *)data, size);
}

YMODEM_STATUS_EN transmitter::receive_data_handler(uint8_t *data, size_t size)
{
  return YMODEM_OK;
}

void transmitter::init(SERIALPORT_INFO_ST *serialport_info)
{
  SerialportInfo = *serialport_info;
  SerialPort.moveToThread(this);

  YMODE_DRIVER_ST YmodemDriver;
  YmodemDriver.read_block = Read_Block;
  YmodemDriver.write = Write;
  YmodemDriver.receive_data_handler = receive_data_handler;
  YMODEM_Init(&Ymodem, &YmodemDriver);
}

void transmitter::run(void)
{
  if(!OpenSerialport(SerialPort, &SerialportInfo))
  {
    qDebug() << "open " + SerialPort.portName() + " failed!";
    return;
  }

  FILE_INFO_ST file_info;
  while(!QuitFlag)
  {
    if(!SerialPort.isOpen())
    {
      msleep(10);
      continue;
    }

    ReceiveFile(&Ymodem, &file_info);
  }

  if(SerialPort.isOpen())
  {
    SerialPort.close();
  }
}

void transmitter::stop(void)
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
