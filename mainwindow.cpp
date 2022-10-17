#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>

#include "newsessiondialog.h"
#include "serialportassistant.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->actionNewSession, &QAction::triggered,
          this, &MainWindow::slotNewSession);

  connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
          this, &MainWindow::slotCloseTabwidgetPage);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotNewSession(void)
{
  NewSessionDialog NewSession;
  if(QDialog::Accepted == NewSession.exec())
  {
    int index = ui->tabWidget->addTab(new SerialportAssistant(ui->statusbar, ui->tabWidget), "串口调试助手");
    ui->tabWidget->setCurrentIndex(index);

    ui->statusbar->showMessage("session type: " + NewSession.SessionType());
  }
}

void MainWindow::slotCloseTabwidgetPage(int index)
{
  delete ui->tabWidget->widget(index);
}


