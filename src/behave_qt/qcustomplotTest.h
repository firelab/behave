#ifndef QCUSTOMPLOTTEST_HEADER
#define QCUSTOMPLOTTEST_HEADER

#include "behaveVector.h"

#ifdef _MSC_VER
#pragma warning(push, 3)
#endif
#include "ui_qcustomplot_test.h"
#include <QtWidgets/QMainWindow>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

class QcustomplotTest : public QMainWindow
{
    Q_OBJECT

public:
    QcustomplotTest(QWidget* parent = 0);
    ~QcustomplotTest();

public slots:
  

private:
    //void establishConnections();
  
    Ui::qcustomplot_testClass ui;
    BehaveVector behaveVector;
};

#endif // QCUSTOMPLOTTEST_HEADER
