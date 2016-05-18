#ifndef BEHAVE_TREE_HEADER
#define BEHAVE_TREE_HEADER

#ifdef _MSC_VER
#pragma warning(push, 3)      
#include <QTreeWidget>
#pragma warning(pop)
#endif

class BehaveTree: public QTreeWidget
{
    Q_OBJECT

public:
    BehaveTree(QWidget* parent);
    void establishConnections();

signals :
    void currentIDChanged(int);

public slots:
    void currentItemChanged(QTreeWidgetItem* current);
};

#endif BEHAVE_TREE_HEADER
