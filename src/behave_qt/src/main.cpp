#include <QtWidgets/QApplication>
#include "behave_qt_main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BehaveQtMainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
