#include "qcustomplotTest.h"

#ifdef _MSC_VER
#pragma warning(push, 3)   
#endif

QcustomplotTest::QcustomplotTest(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QCustomPlot* customPlot = ui.customPlot;

    const int NO_OF_ROWS = 10; // this is the number of "graphs" being made 
    const int NO_OF_COLS = 200; // this is the number of sample points used the generate each graph 

    QVector<double> x(NO_OF_COLS); // stores the values in the domain interval for the plot

    QVector<QVector<double>> y(NO_OF_ROWS); // stores the values in the range for each graph
    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        y[i].resize(NO_OF_COLS);
    }

    //double oneHourMoisture = 0;
    //double oneHourMoistureMin = 1;
    //double oneHourMoistureMax = 10;

    double windSpeed = 0;
    double windSpeedMin = 0;
    double windSpeedMax = 10;

    
    int fuelModelNumberMin = 1;
    int fuelModelNumberMax = 10;

    int fuelModelNumber = 0;

    double incrementAmount = 0; // the amount that the x variable's value increases each iteration
    double xMin = 0; // minimum value for x variable in the interval
    double xMax = 0; // maxiimum value for x variable in the interval

    //xMin = oneHourMoistureMin;
    //xMax = oneHourMoistureMax;
    xMin = fuelModelNumberMin;
    xMax = fuelModelNumberMax;

    incrementAmount = (xMax - xMin) / (NO_OF_ROWS - 1);

    //oneHourMoisture = xMin;
    fuelModelNumber = fuelModelNumberMin;


    behaveVector.resize(NO_OF_ROWS);

    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        //oneHourMoisture += incrementAmount; // Update the value of the secondary independently ranged variable
        //behaveVector.behave[i].updateSurfaceInputs(1, oneHourMoisture, .07, .08, .60, .90, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, 0, 0, 0, 0, 0, 0);
        //behaveVector[i].updateSurfaceInputs(fuelModelNumber, 6, 7, 8, 60, 90, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, 0, 0, 0, 0, 0, 0);
        behaveVector[i].updateSurfaceInputs(fuelModelNumber, 6, 7, 8, 60, 90, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, 0, 0, 0, 0, 0, 0);
        customPlot->addGraph(); // add a graph to the plot
        fuelModelNumber += (int)incrementAmount; // Update the value of the secondary independently ranged variable
    }

    xMin = windSpeedMin;
    xMax = windSpeedMax;
    incrementAmount = (xMax - xMin) / (NO_OF_COLS - 1); // this is the amount that the x variable's value increases each iteration
    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        windSpeed = 0; // reset the x variable's value for next iteration
        for (int j = 0; j < NO_OF_COLS; j++)
        {
            windSpeed += incrementAmount; // update the x variable's value
            x[j] = windSpeed; // x goes from xMin to xMax
            behaveVector[i].setWindSpeed(windSpeed); // This is where you make use of the x value in the function being graphed
            double directionOfInterest = 0;
            behaveVector[i].doSurfaceRunInDirectionOfInterest(directionOfInterest);
            y[i][j] = behaveVector[i].getSurfaceFireSpreadRate(); // This updates the y value
        }
    }

    //for (int i = 0; i < NO_OF_ROWS; i++)
    //{
    //    windSpeed = 0. + (i * 10);
    //    behaveVector[i].updateSurfaceInputs(1, oneHourMoisture, .07, .08, .60, .90, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, 0, 0, 0, 0, 0, 0);
    //    customPlot->addGraph();
    //}


    //oneHourMoisture = oneHourMoistureMin;
    //xMin = oneHourMoistureMin;
    //xMax = oneHourMoistureMax;

    //incrementAmount = (xMax - xMin) / (NO_OF_COLS - 1);
    //for (int i = 0; i < NO_OF_ROWS; i++)
    //{
    //    oneHourMoisture = xMin;
    //    for (int j = 0; j < NO_OF_COLS; j++)
    //    {
    //        oneHourMoisture += incrementAmount;
    //        x[j] = oneHourMoisture; // x goes from 0.01 to 0.10 one hour moisture
    //        behaveVector[i].setMoistureOneHour(oneHourMoisture);
    //        y[i][j] = behaveVector[i].calculateSurfaceFireForwardSpreadRate();
    //    }
    //}

    QVector<QPen> pen;
    pen.resize(10);

    pen[0].setColor(Qt::black);
    pen[1].setColor(Qt::blue);
    pen[2].setColor(Qt::cyan);
    pen[3].setColor(Qt::green);
    pen[4].setColor(Qt::red);
    pen[5].setColor(Qt::magenta);
    pen[6].setColor("brown");
    pen[7].setColor("darkorange");
    pen[8].setColor("darkcyan");
    pen[9].setColor("slategray");

    customPlot->xAxis->setLabel("wind speed, uplsope (mi/hr)");
    customPlot->yAxis->setLabel("spread rate (ch/hr)");
    //customPlot->xAxis->setRange(-1, 1);
    //customPlot->xAxis->setRange(0, 10);
    //customPlot->yAxis->setRange(0, 1);
    //customPlot->yAxis->setRange(0, 300);

    for (int i = 0; i < NO_OF_ROWS; i++)
    {
        pen[i].setWidth(3);
        customPlot->graph(i)->setPen(pen[i]);
        customPlot->graph(i)->setData(x, y[i]);
        QString numberString = QString::number(i + 1);
        QString nameString = "Fuel model " + numberString;
        customPlot->graph(i)->setName(nameString);
    }

    customPlot->xAxis->setRange(xMin, xMax);
    customPlot->yAxis->rescale(true);
    customPlot->plotLayout()->addElement(0, 1, customPlot->legend);
    customPlot->legend->setMaximumSize(80, QWIDGETSIZE_MAX);
    customPlot->legend->setVisible(true);
    customPlot->replot();

}

QcustomplotTest::~QcustomplotTest()
{

}

#ifdef _MSC_VER
#pragma warning(push, 3)   
#endif
