#include "grapheditpanel.h"
#include "ui_grapheditpanel.h"

GraphEditPanel::GraphEditPanel(QWidget *parent) : QWidget(parent), ui(new Ui::GraphEditPanel)
{
  ui->setupUi(this);
}

GraphEditPanel::~GraphEditPanel()
{
  delete ui;
}
