#include "newsessiondialog.h"

#include "ui_NewSessionWizard.h"


NewSessionDialog::NewSessionDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewSessionDialog)
{
  ui->setupUi(this);
}

NewSessionDialog::~NewSessionDialog()
{
  delete ui;
}

QString NewSessionDialog::SessionType(void)
{
  return ui->comboBox->currentText();
}
