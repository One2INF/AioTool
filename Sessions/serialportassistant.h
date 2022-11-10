#ifndef SERIALPORTASSISTANT_H
#define SERIALPORTASSISTANT_H

#include <QWidget>
#include <QSerialPort>
#include <QStatusBar>
#include <QTimer>

#include "serialportthread.h"


namespace Ui {
  class SerialportAssistant;
}

class SerialportAssistant : public QWidget
{
  Q_OBJECT

public:
  explicit SerialportAssistant(QStatusBar *statusbar, QWidget *parent = nullptr);
  ~SerialportAssistant();

  serialportthread SerialportThread;

private:
  QStatusBar *statusbar;
  Ui::SerialportAssistant *ui;

  /* 串口操作函数 */
  bool OpenPort(void);

  void SignalSlotConnect(void);

  void signalDataReceived(QByteArray &data);
signals:

private slots:
  void slotOpenPortButtonClicked(bool flag);
  void slotUpdateSerialList(void);
  void slotSendInputBoxData(void);
  void slotUpdateReceiveTexteditor(QByteArray data);
  void slotClearReceivedData(void);
  void slotClearInputBoxData(void);
};

#endif // SERIALPORTASSISTANT_H
