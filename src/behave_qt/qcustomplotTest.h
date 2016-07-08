#ifndef QCUSTOMPLOTTEST_HEADER
#define QCUSTOMPLOTTEST_HEADER

#include "behaveVector.h"

#include "ui_qcustomplot_test.h"
#include "qtIncludesSilencedW4.h"

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
