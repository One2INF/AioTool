#include "transmitter.h"

#include <QtSerialPort>
#include <QDebug>

#include "serialportthread.h"


serialportthread* transmitter::SerialportHandle;

transmitter::transmitter(serialportthread *serialport, QObject *parent) : QThread(parent)
{
  SerialportHandle = serialport;

  YMODE_DRIVER_ST YmodemDriver;
  YmodemDriver.read_block = Read_Block;
  YmodemDriver.write = Write;
  YmodemDriver.receive_data_handler = receive_data_handler;
  YMODEM_Init(&Ymodem, &YmodemDriver);
}

transmitter::~transmitter(void)
{
  stop();
  terminate();
  wait();
}

void transmitter::Send(const QString file_name, const quint32 send_times)
{
  if(!SerialportHandle->isOpen())
  {
    qDebug() << "port not open!";
    return;
  }

  FileName = file_name;
  SendTimes = send_times;

  start();
}

size_t transmitter::Read_Block(uint8_t *data, size_t size, uint32_t timeout)
{
  size = SerialportHandle->read_block((char*)data, size, timeout);
  qDebug() << "Ymode read:" << QByteArray((char*)data, size);
  return size;
}

size_t transmitter::Write(const uint8_t *data, size_t size)
{
  qDebug() << "Ymode write:" << data[0] << data[1];
  return SerialportHandle->write((const char*)data, size);
}

YMODEM_STATUS_EN transmitter::receive_data_handler(size_t packet_offset, const uint8_t *data, size_t size)
{
  return YMODEM_OK;
}

void transmitter::run(void)
{
  QFile file(FileName);
  if(!file.open(QIODevice::ReadOnly))
  {
    qDebug() << "open file " << FileName << "failed!";
    qDebug() << file.errorString();
    return;
  }

  uint32_t TransformCount = 0;
  uint32_t SuccessCount = 0;
  uint32_t ErrorCount = 0;
  QString Ret;

  QFileInfo File_Info(file);
  QByteArray str_data = File_Info.fileName().toLatin1();
  strncpy(FileInfo.name, str_data.data(), FILE_NAME_LENGTH);
  FileInfo.size = file.size();
  qDebug() << FileInfo.name << FileInfo.size << SendTimes;
  QByteArray data = file.readAll();

  while(SendTimes--)
  {
    //ReceiveFile(&Ymodem, &file_info);

    ++TransformCount;

    qDebug() << "times: " << TransformCount;

    if(YMODEM_OK == YMODEM_SendFile(&Ymodem, &FileInfo, (uint8_t*)data.constData(), file.size()))
    {
      ++SuccessCount;
      Ret = "Successful";
    }
    else
    {
      ++ErrorCount;
      Ret = "Failed";
    }
    QString text = QString("%1: ").arg(TransformCount) + Ret +
                  QString(", S: %1: E: %2").arg(SuccessCount).arg(ErrorCount);

    emit UpdateText(text);
  }
  file.close();
}

void transmitter::stop(void)
{
  QuitFlag = true;
}
