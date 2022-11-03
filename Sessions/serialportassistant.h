#ifndef SERIALPORTASSISTANT_H
#define SERIALPORTASSISTANT_H

#include <QWidget>
#include <QSerialPort>
#include <QStatusBar>
#include <QTimer>


namespace Ui {
  class SerialportAssistant;
}

class SerialportAssistant : public QWidget
{
  Q_OBJECT

public:
  explicit SerialportAssistant(QStatusBar *statusbar, QWidget *parent = nullptr);
  ~SerialportAssistant();

public slots:

private:
  QStatusBar *statusbar;
  QSerialPort SerialPort;
  Ui::SerialportAssistant *ui;

  QTimer timer;
  QByteArray recv_data;

  /* 串口操作函数 */
  bool OpenPort(void);

  void SignalSlotConnect(void);

private slots:

  void slotOpenPortButtonClicked(bool flag);
  void slotUpdateSerialList(void);
  void slotReadData(void);
  void slotSendData(void);
  void slotUpdateReceiveTexteditor(void);
  void slotClearReceivedData(void);
  void slotClearSendData(void);
};

#endif // SERIALPORTASSISTANT_H
