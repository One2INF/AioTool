#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>

#include "transmitter.h"


QSerialPort transmitter::SerialPort;


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  SERIALPORT_INFO_ST serialport_info;

  if(argc == 3)
  {
    serialport_info.portname = (QString)argv[1];
    serialport_info.baudrate = ((QString)argv[2]).toInt();
  }
  else
  {
    qDebug() << QString("Usage: %1 portnum baudrate!\r\n").arg(argv[0]);
    exit(EXIT_FAILURE);
  }

  transmitter transmitter;
  transmitter.init(&serialport_info);
  transmitter.start();

  setbuf(stdout, NULL);
  return a.exec();
}
