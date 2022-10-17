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
  SignalSlotConnect();
  slotUpdateSerialList();
}

SerialportAssistant::~SerialportAssistant()
{
  delete ui;
}

void SerialportAssistant::SignalSlotConnect(void)
{
  connect(ui->comboBox_Port, &ComboBoxExt::clicked,
          this, &SerialportAssistant::slotUpdateSerialList);

  /* 按钮 */
  connect(ui->pushButton_OpenPort, &QPushButton::clicked,
          this, &SerialportAssistant::slotOpenPortButtonClicked);

  /* 串口类 */
  connect(&SerialPort, &QSerialPort::readyRead,
          this, &SerialportAssistant::slotReadData);
}

void SerialportAssistant::slotOpenPortButtonClicked(bool flag)
{
  if(SerialPort.isOpen())
  {
    SerialPort.close();
    statusbar->showMessage("close " + SerialPort.portName());
    ui->pushButton_OpenPort->setChecked(true);
    return;
  }

  if(OpenPort())
    ui->pushButton_OpenPort->setChecked(true);
}

bool SerialportAssistant::OpenPort(void)
{
  SerialPort.setPortName(ui->comboBox_Port->currentText());
  SerialPort.setBaudRate(ui->comboBox_Baudrate->currentData().toInt());

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

  if(SerialPort.open(QIODevice::ReadWrite))
  {
    statusbar->showMessage("open " + SerialPort.portName() + " succeed!");
    return true;
  }
  else
  {
    statusbar->showMessage("open " + SerialPort.portName() + " failed!");
    return false;
  }
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

}

void SerialportAssistant::slotReadData(void)
{
  QByteArray data = SerialPort.readAll();
  UpdateReceiveTexteditor(QString::fromUtf8(data));
}
