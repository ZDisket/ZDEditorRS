#ifndef GENERICTABLEPAGE_H
#define GENERICTABLEPAGE_H
#include <QTableWidget>
#include <QWidget>
#include <ext/Signal.h>
#include <idatabasepp.hpp>
namespace Ui {
class GenericTablePage;
}

class GenericTablePage : public QWidget
{
    Q_OBJECT

public:

    explicit GenericTablePage(QWidget *parent = 0);
    ~GenericTablePage();

    Gallant::Signal1<QTableWidgetItem*> sigRollback;
    QTableWidget* pTable;
    ZTable* pDBTable;
    ZTable mDBTable;

private slots:
    void on_tableW_windowIconChanged(const QIcon &icon);

    void on_tableW_itemChanged(QTableWidgetItem *item);

private:
    bool Eval(const GString & vstr, const EDataType & type);
    Ui::GenericTablePage *ui;

    void FindUndo(QTableWidgetItem *pItem);
    struct Edit{
        QString txt;
        // 1st is row, 2nd is column
        pair<int,int> pos;
    };
    vector<Edit> Edits;
};

#endif // GENERICTABLEPAGE_H
