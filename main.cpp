#include "mainwindow.h"
#include <QApplication>
#include <QLineEdit>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/new/images/icons/core_browser_icon.png"));
    w.show();

    return app.exec();
}
