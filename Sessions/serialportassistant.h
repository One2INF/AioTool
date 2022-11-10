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
  QList <QByteArray> listRecvData;
  QList <QByteArray> listWriteData;
  QSerialPort SerialPort;

private:
  QStatusBar *statusbar;
  Ui::SerialportAssistant *ui;

  QTimer ReadTimer;
  QTimer WriteTimer;

  QByteArray recv_data;

  /* 串口操作函数 */
  bool OpenPort(void);

  void SignalSlotConnect(void);

signals:
  void signalDataReceived(QByteArray &data);

private slots:
  void slotOpenPortButtonClicked(bool flag);
  void slotUpdateSerialList(void);
  void slotReadData(void);
  void slotSendData(void);
  void slotUpdateReceiveTexteditor(void);
  void slotClearReceivedData(void);
  void slotClearSendData(void);

  void slotWriteData(void);
};

#endif // SERIALPORTASSISTANT_H
