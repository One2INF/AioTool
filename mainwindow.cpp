#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include "newsessiondialog.h"
#include "serialportassistant.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->actionNewSession, &QAction::triggered,
          this, &MainWindow::slotNewSession);

  connect(ui->actionAboutMe, &QAction::triggered,
          this, &MainWindow::slotAboutMe);

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

void MainWindow::slotAboutMe(void)
{
  QString msgAuthor = QString("Author: One2INF<p>");
  QString msgGitee = QString("Gitee: <a href=https://gitee.com/One2INF>https://gitee.com/One2INF</a><p>");
  QString msgGithub = QString("Github: <a href=https://github.com/One2INF>https://github.com/One2INF</a><p>");
  QString msgEmail = QString("Email: <a href=mailto: 1871750676@qq.com>1871750676@qq.com</a><p>");
  QString msg = msgAuthor + msgGitee + msgEmail;

  QMessageBox::about(nullptr, "About Me", msg);
}
