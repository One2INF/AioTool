#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextCodec>

#include "transmitter.h"


QSerialPort transmitter::SerialPort;


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowTitle("AioTool - One2INF");
  w.show();

  QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

  setbuf(stdout, NULL);
  return a.exec();
}
