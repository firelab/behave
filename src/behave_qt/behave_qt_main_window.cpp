#include "behave_qt_main_window.h"

BehaveQtMainWindow::BehaveQtMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.behaveTree, SIGNAL(currentIDChanged(int)), ui.mainWindowStackedWidget, SLOT(setCurrentIndex(int)));
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

    connect(ui.fuelModelLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(fuelInputChanged(const QString&)));
    ui.fuelModelLineEdit->setProperty("ID", 0);
}

void BehaveQtMainWindow::fuelInputChanged(const QString& text)
{
    int fuelModelNumber = text.toInt();
}

BehaveQtMainWindow::~BehaveQtMainWindow()
{

}

