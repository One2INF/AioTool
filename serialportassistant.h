#ifndef SERIALPORTASSISTANT_H
#define SERIALPORTASSISTANT_H

#include <QWidget>
#include <QSerialPort>
#include <QStatusBar>


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

  /* 串口操作函数 */
  bool OpenPort(void);

  void SignalSlotConnect(void);
  void UpdateReceiveTexteditor(QString text);

private slots:

  void slotOpenPortButtonClicked(bool flag);
  void slotUpdateSerialList(void);
  void slotReadData(void);
  void slotSendData(void);
  void slotClearReceivedData(void);
  void slotClearSendData(void);
};

#endif // SERIALPORTASSISTANT_H
