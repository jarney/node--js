#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QPushButton>

namespace NodeJS {
  namespace gui {

class BreadcrumbsWidget : public QWidget {
public:
    BreadcrumbsWidget();
    ~BreadcrumbsWidget();
    void addPage(QWidget * crumb);
    void removePage(int index);
    QWidget *getPage();

private:
    std::unique_ptr<QVBoxLayout> layout;

    QScrollArea *scroll_area;
    QHBoxLayout *hbox_layout;
    std::vector<QWidget *> pages;
    std::vector<QWidget *> buttons;
};

  } // End gui
} // End NodeJS
