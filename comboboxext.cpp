#include "comboboxext.h"

#include <QMouseEvent>


ComboBoxExt::ComboBoxExt(QWidget *parent)
  : QComboBox{parent}
{

}

void ComboBoxExt::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
  {
    emit clicked();
  }

  QComboBox::mousePressEvent(event);
}
