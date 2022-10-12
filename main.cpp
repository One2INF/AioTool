#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>

#include "transmitter.h"
#include "ymodem.h"


QSerialPort transmitter::SerialPort;


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  SERIALPORT_INFO_ST serialport_info;
  FILE_INFO_ST file_info;

  if(argc == 4)
  {
    serialport_info.portname = (QString)argv[1];
    serialport_info.baudrate = ((QString)argv[2]).toInt();
    strncpy(file_info.name, argv[3], sizeof(file_info.name));
  }
  else
  {
    qDebug() << QString("Usage: %1 portnum baudrate!\r\n").arg(argv[0]);
    exit(EXIT_FAILURE);
  }

  transmitter transmitter;
  transmitter.init(&serialport_info, &file_info);
  transmitter.start();

  QObject::connect(&transmitter, &transmitter::UpdateText, &w, &MainWindow::TexteditUpdateCount);

  setbuf(stdout, NULL);
  return a.exec();
}
