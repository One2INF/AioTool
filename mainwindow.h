#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void slotNewSession(void);
  void slotAboutMe(void);

private:
   Ui::MainWindow *ui;

private slots:
  void slotCloseTabwidgetPage(int index);
};
#endif // MAINWINDOW_H
