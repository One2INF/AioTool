#ifndef COMBOBOXEXT_H
#define COMBOBOXEXT_H

#include <QComboBox>

class ComboBoxExt : public QComboBox
{
  Q_OBJECT
public:
  explicit ComboBoxExt(QWidget *parent = nullptr);
  void mousePressEvent(QMouseEvent *event);

signals:
  void clicked(void);
};

#endif // COMBOBOXEXT_H
