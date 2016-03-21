#include "behave_qtree_widget.h"

BehaveTree::BehaveTree(QWidget* parent)
    : QTreeWidget(parent)
{
    establishConnections();
}

void BehaveTree::currentItemChanged(QTreeWidgetItem *item)
{
    int id = item->data(0, Qt::UserRole).toInt();
    emit currentIDChanged(id);
}

void BehaveTree::establishConnections()
{
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(currentItemChanged(QTreeWidgetItem*)));
}
