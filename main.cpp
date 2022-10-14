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
  w.setWindowTitle("AioTool - One2INF");
  w.show();

  setbuf(stdout, NULL);
  return a.exec();
}
