#ifndef GRAPHEDITPANEL_H
#define GRAPHEDITPANEL_H

#include <QWidget>

namespace Ui {
class GraphEditPanel;
}

class GraphEditPanel : public QWidget
{
  Q_OBJECT

public:
  explicit GraphEditPanel(QWidget *parent = nullptr);
  ~GraphEditPanel();

private:
  Ui::GraphEditPanel *ui;
};

#endif  // GRAPHEDITPANEL_H
