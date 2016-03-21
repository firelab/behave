#ifndef BEHAVE_QT_GUI_H
#define BEHAVE_QT_GUI_H

#include <QtWidgets/QMainWindow>
#include "ui_behave_qt_main_window.h"
#include "src/behave.h"
#include "src/behave_qt/behave_qt_main_window_enums.h"

class BehaveQtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    BehaveQtMainWindow(QWidget* parent = 0);
    ~BehaveQtMainWindow();

public slots:
    void fuelModelInputChanged(const QString& text);

private:
    void establishConnections();
    void populateTree();

    Ui::behave_qt_main_windowClass ui;
    Behave behave;
};
#endif // BEHAVE_QT_GUI_H
