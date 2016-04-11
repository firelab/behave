#include "behave_qt_main_window.h"

BehaveQtMainWindow::BehaveQtMainWindow(QWidget *parent)
    : QMainWindow(parent),
      behave(fuelModels)
{
    ui.setupUi(this);
    populateTree();
    establishConnections();
}

BehaveQtMainWindow::~BehaveQtMainWindow()
{

}

void BehaveQtMainWindow::establishConnections()
{
    connect(ui.behaveTree, SIGNAL(currentIDChanged(int)), ui.mainWindowStackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui.fuelModelLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(fuelModelInputChanged(const QString&)));
    connect(ui.oneHourMoistureLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(oneHourMoistureInputChanged(const QString&)));
    connect(ui.tenHourMoistureLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(tenHourMoistureInputChanged(const QString&)));
    connect(ui.hundredHourMoistureLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(hundredHourMoistureInputChanged(const QString&)));
    connect(ui.liveHerbaceousMoistureLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(liveHerbaceousMoistureInputChanged(const QString&)));
    connect(ui.liveWoodyLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(liveWoodyMoistureInputChanged(const QString&)));
    connect(ui.windSpeedLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(windSpeedInputChanged(const QString&)));
    connect(ui.slopeLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slopeInputChanged(const QString&)));

    connect(ui.calculateButton, SIGNAL(clicked()), this, SLOT(calculateClicked()));

}

void BehaveQtMainWindow::populateTree()
{
    QTreeWidgetItem *inputItem = new QTreeWidgetItem;
    inputItem->setText(0, tr("Input"));
    inputItem->setData(0, Qt::UserRole, BehaveMainWindowStackedWidget::MAIN_INPUT);
    ui.behaveTree->addTopLevelItem(inputItem);
    QTreeWidgetItem *surfaceInputItem = new QTreeWidgetItem;
    surfaceInputItem->setText(0, tr("Surface Fire Spread"));
    surfaceInputItem->setData(0, Qt::UserRole, BehaveMainWindowStackedWidget::SURFACE_INPUT);
    inputItem->addChild(surfaceInputItem);
    QTreeWidgetItem *FuelInputItem = new QTreeWidgetItem;
    FuelInputItem->setText(0, tr("Fuel"));
    FuelInputItem->setData(0, Qt::UserRole, BehaveMainWindowStackedWidget::FUEL_INPUT);
    surfaceInputItem->addChild(FuelInputItem);
}

// Slot
void BehaveQtMainWindow::fuelModelInputChanged(const QString& text)
{
    int fuelModelNumber = text.toInt();
    behave.setFuelModelNumber(fuelModelNumber);
}

// Slot
void BehaveQtMainWindow::oneHourMoistureInputChanged(const QString& text)
{
    double oneHourMoisture = text.toDouble() / 100.0;
    behave.setMoistureOneHour(oneHourMoisture);
}

// Slot
void BehaveQtMainWindow::tenHourMoistureInputChanged(const QString& text)
{
    double tenHourMoisture = text.toDouble() / 100.0;
    behave.setMoistureTenHour(tenHourMoisture);
}

// Slot
void BehaveQtMainWindow::hundredHourMoistureInputChanged(const QString& text)
{
    double hundredHourMoisture = text.toDouble() / 100.0;
    behave.setMoistureHundredHour(hundredHourMoisture);
}

// Slot
void BehaveQtMainWindow::liveHerbaceousMoistureInputChanged(const QString& text)
{
    double liveHerbaceousMoisture = text.toDouble() / 100.0;
    behave.setMoistureLiveHerbaceous(liveHerbaceousMoisture);
}

// Slot
void BehaveQtMainWindow::liveWoodyMoistureInputChanged(const QString& text)
{
    double liveWoodyMoisture = text.toDouble() / 100.0;
    behave.setMoistureLiveWoody(liveWoodyMoisture);
}

// Slot
void BehaveQtMainWindow::windSpeedInputChanged(const QString& text)
{
    double windSpeed = text.toDouble();
    behave.setWindSpeed(windSpeed);
}

// Slot
void BehaveQtMainWindow::slopeInputChanged(const QString& text)
{
    double slope = text.toDouble();
    behave.setSlope(slope);
}

// Slot
void BehaveQtMainWindow::calculateClicked()
{
    double spreadRate = behave.calculateSurfaceFireForwardSpreadRate();
    ui.spreadRateLineEdit->setText(QString::number(spreadRate, 10, 2));
}

