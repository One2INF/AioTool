#include "ymodemwidget.h"
#include "ui_ymodemwidget.h"

#include <QPushButton>
#include <QFileDialog>
#include <QDebug>

#include "transmitter.h"
#include "serialportassistant.h"


YmodemWidget::YmodemWidget(QWidget *parent) :
  QDockWidget(parent),
  ui(new Ui::YmodemWidget)
{
  ui->setupUi(this);

  transmitter_ymodem = new transmitter(&((SerialportAssistant*)parent)->SerialportThread);

  connect(ui->pushButtonOpenFile, &QPushButton::clicked, this, &YmodemWidget::slotOpenFile);
  connect(ui->pushButtonSend, &QPushButton::clicked, this, &YmodemWidget::slotSend);
}

void YmodemWidget::slotOpenFile(bool flag)
{
  QString FileName = QFileDialog::getOpenFileName(this, tr("Select File to send"), ".", tr("File (*)"));
  if(FileName.isNull())
    return;

  ui->lineEdit->setText(FileName);
  ui->textBrowser->append("Select file:\r\n" + FileName);
}

void YmodemWidget::slotSend(bool flag)
{
  QString FileName = ui->lineEdit->text();
  quint32 times = ui->lineEdit_SendTimes->text().toUInt();
  transmitter_ymodem->Send(FileName, times);
}

YmodemWidget::~YmodemWidget()
{
  delete ui;
}
