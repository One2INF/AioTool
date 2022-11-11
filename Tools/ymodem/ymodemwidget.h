#ifndef YMODEMWIDGET_H
#define YMODEMWIDGET_H

#include <QDockWidget>
#include "transmitter.h"
#include "serialportthread.h"


namespace Ui {
  class YmodemWidget;
}

class YmodemWidget : public QDockWidget
{
  Q_OBJECT

public:
  YmodemWidget(QWidget *parent = nullptr);
  ~YmodemWidget();

private:
  Ui::YmodemWidget *ui;
  transmitter *transmitter_ymodem;

private slots:
  void slotOpenFile(bool flag);
  void slotSend(bool flag);
};

#endif // YMODEMWIDGET_H
