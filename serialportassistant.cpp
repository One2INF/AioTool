#include "serialportassistant.h"
#include "ui_serialportassistant.h"

#include <QAbstractItemView>
#include <QSerialPortInfo>
#include <QDebug>


SerialportAssistant::SerialportAssistant(QStatusBar *status_bar, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SerialportAssistant)
{
  ui->setupUi(this);

  statusbar = status_bar;

  /* 设置按钮不自复位 */
  ui->pushButton_OpenPort->setCheckable(true);

  SignalSlotConnect();
  slotUpdateSerialList();
}

SerialportAssistant::~SerialportAssistant()
{
  delete ui;
qDebug() << "SerialportAssistant destruction";
  if(SerialPort.isOpen())
  {
    SerialPort.close();
    statusbar->showMessage("close " + SerialPort.portName() + ".");
  }
}

void SerialportAssistant::SignalSlotConnect(void)
{
  connect(ui->comboBox_Port, &ComboBoxExt::clicked,
          this, &SerialportAssistant::slotUpdateSerialList);

  /* 按钮 */
  connect(ui->pushButton_OpenPort, &QPushButton::clicked,
          this, &SerialportAssistant::slotOpenPortButtonClicked);
  connect(ui->pushButton_ClearReceive, &QPushButton::clicked,
          this, &SerialportAssistant::slotClearReceivedData);
  connect(ui->pushButton_Send, &QPushButton::clicked,
          this, &SerialportAssistant::slotSendData);
  connect(ui->pushButton_ClearSend, &QPushButton::clicked,
          this, &SerialportAssistant::slotClearSendData);

  /* 串口类 */
  connect(&SerialPort, &QSerialPort::readyRead,
          this, &SerialportAssistant::slotReadData);
}

void SerialportAssistant::slotOpenPortButtonClicked(bool flag)
{
  if(SerialPort.isOpen())
  {
    SerialPort.close();
    statusbar->showMessage("close " + SerialPort.portName() + ".");
    return;
  }

  if(OpenPort())
  {
    ui->pushButton_OpenPort->setChecked(true);
    statusbar->showMessage("open " + SerialPort.portName() + " succeed.");
  }
  else
  {
    ui->pushButton_OpenPort->setChecked(false);
    statusbar->showMessage("open " + SerialPort.portName() + " failed.");
  }
}

bool SerialportAssistant::OpenPort(void)
{
  SerialPort.setPortName(ui->comboBox_Port->currentText().split(": ")[0]);
  SerialPort.setBaudRate(ui->comboBox_Baudrate->currentText().toLong());

  QSerialPort::StopBits StopBits = QSerialPort::OneStop;
  if(1.5 == ui->comboBox_StopBit->currentData())
  {
    StopBits = QSerialPort::OneAndHalfStop;
  }
  else if(2 == ui->comboBox_StopBit->currentData())
  {
    StopBits = QSerialPort::TwoStop;
  }
  SerialPort.setStopBits(StopBits);

  QSerialPort::Parity Parity = QSerialPort::NoParity;
  if("奇" == ui->comboBox_CheckBit->currentText())
  {
    Parity = QSerialPort::OddParity;
  }
  else if("偶" == ui->comboBox_CheckBit->currentText())
  {
    Parity = QSerialPort::EvenParity;
  }
  SerialPort.setParity(Parity);

  QSerialPort::FlowControl FlowControl = QSerialPort::NoFlowControl;
  if("RTS/CTS" == ui->comboBox_FlowControl->currentText())
  {
    FlowControl = QSerialPort::HardwareControl;
  }
  else if("XON/XOFF" == ui->comboBox_FlowControl->currentText())
  {
    FlowControl = QSerialPort::SoftwareControl;
  }
  SerialPort.setFlowControl(FlowControl);

  qDebug() << SerialPort.baudRate() << SerialPort.stopBits()
           << SerialPort.parity() << SerialPort.flowControl();

  return SerialPort.open(QIODevice::ReadWrite);
}

void SerialportAssistant::slotUpdateSerialList(void)
{
  ui->comboBox_Port->clear();
  foreach(const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
  {
    ui->comboBox_Port->addItem(Info.portName() + ": " + Info.description());
  }
}

void SerialportAssistant::UpdateReceiveTexteditor(QString text)
{
  ui->textBrowser->insertPlainText(text);
  ui->textBrowser->moveCursor(QTextCursor::End);
}

void SerialportAssistant::slotReadData(void)
{
  QByteArray data = SerialPort.readAll();
  UpdateReceiveTexteditor(QString::fromUtf8(data));
}

void SerialportAssistant::slotSendData(void)
{
  if(SerialPort.isOpen())
  {
    SerialPort.write(ui->plainTextEdit_Send->toPlainText().toLocal8Bit());
  }
}

void SerialportAssistant::slotClearReceivedData(void)
{
  ui->textBrowser->clear();
}

void SerialportAssistant::slotClearSendData(void)
{
  ui->plainTextEdit_Send->clear();
}
