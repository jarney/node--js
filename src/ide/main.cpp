#include "ui_mainwindow2.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //QWidget widget;
    QMainWindow widget;
    Ui::MainWindow ui;
    ui.setupUi(&widget);

    widget.show();
    return app.exec();
}
