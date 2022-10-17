#ifndef NEWSESSIONDIALOG_H
#define NEWSESSIONDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class NewSessionDialog; }
QT_END_NAMESPACE


class NewSessionDialog : public QDialog
{
  Q_OBJECT
public:
  explicit NewSessionDialog(QWidget *parent = nullptr);
  ~NewSessionDialog();

  QString SessionType(void);

private:
  Ui::NewSessionDialog *ui;

signals:

};

#endif // NEWSESSIONDIALOG_H
