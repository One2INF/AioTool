#include "serialportassistant.h"
#include "ui_serialportassistant.h"

#include <QAbstractItemView>
#include <QSerialPortInfo>
#include <QTimer>
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

  if(SerialportThread.isOpen())
  {
    SerialportThread.close();
    statusbar->showMessage("close " + ui->comboBox_Port->currentText() + ".");
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
          this, &SerialportAssistant::slotSendInputBoxData);
  connect(ui->pushButton_ClearSend, &QPushButton::clicked,
          this, &SerialportAssistant::slotClearInputBoxData);

  connect(&SerialportThread, &serialportthread::signalReadyRead,
          this, &SerialportAssistant::slotUpdateReceiveTexteditor);
}

void SerialportAssistant::slotOpenPortButtonClicked(bool flag)
{
  if(SerialportThread.isOpen())
  {
    SerialportThread.close();
    ui->pushButton_OpenPort->setText("打开串口");
    ui->pushButton_OpenPort->setStyleSheet("background-color:none");
    statusbar->showMessage("close " + ui->comboBox_Port->currentText() + ".");
    return;
  }

  if(OpenPort())
  {
    ui->pushButton_OpenPort->setChecked(true);
    ui->pushButton_OpenPort->setText("关闭串口");
    ui->pushButton_OpenPort->setStyleSheet("background-color:red");
    statusbar->showMessage("open " + ui->comboBox_Port->currentText() + " succeed.");
  }
  else
  {
    ui->pushButton_OpenPort->setChecked(false);
    statusbar->showMessage("open " + ui->comboBox_Port->currentText() + " failed.");
  }
}

bool SerialportAssistant::OpenPort(void)
{
  SERAILPORT_CONFIG_ST SerialportConfig;

  SerialportConfig.portname = ui->comboBox_Port->currentText().split(": ")[0];
  SerialportConfig.baudrate = ui->comboBox_Baudrate->currentText().toLong();

  QSerialPort::StopBits StopBits = QSerialPort::OneStop;
  if(1.5 == ui->comboBox_StopBit->currentText().toFloat())
  {
    StopBits = QSerialPort::OneAndHalfStop;
  }
  else if(2 == ui->comboBox_StopBit->currentText().toFloat())
  {
    StopBits = QSerialPort::TwoStop;
  }
  SerialportConfig.stopbits = StopBits;

  QSerialPort::Parity Parity = QSerialPort::NoParity;
  if("奇" == ui->comboBox_CheckBit->currentText())
  {
    Parity = QSerialPort::OddParity;
  }
  else if("偶" == ui->comboBox_CheckBit->currentText())
  {
    Parity = QSerialPort::EvenParity;
  }
  SerialportConfig.parity = Parity;

  QSerialPort::FlowControl FlowControl = QSerialPort::NoFlowControl;
  if("RTS/CTS" == ui->comboBox_FlowControl->currentText())
  {
    FlowControl = QSerialPort::HardwareControl;
  }
  else if("XON/XOFF" == ui->comboBox_FlowControl->currentText())
  {
    FlowControl = QSerialPort::SoftwareControl;
  }
  SerialportConfig.flowcontrol = FlowControl;

  return SerialportThread.open(&SerialportConfig);
}

void SerialportAssistant::slotUpdateSerialList(void)
{
  ui->comboBox_Port->clear();
  foreach(const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    ui->comboBox_Port->addItem(Info.portName() + ": " + Info.description());
}

void SerialportAssistant::slotUpdateReceiveTexteditor(QByteArray data)
{
  ui->textBrowser->moveCursor(QTextCursor::End);
  ui->textBrowser->insertPlainText(QString::fromUtf8(data));
}


void SerialportAssistant::slotSendInputBoxData(void)
{
  if(SerialportThread.isOpen())
  {
    QByteArray data = ui->plainTextEdit_Send->toPlainText().toUtf8();
    SerialportThread.write(data.constData(), data.size());
  }
}

void SerialportAssistant::slotClearReceivedData(void)
{
  ui->textBrowser->clear();
}

void SerialportAssistant::slotClearInputBoxData(void)
{
  ui->plainTextEdit_Send->clear();
}
