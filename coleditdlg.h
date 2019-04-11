#ifndef COLEDITDLG_H
#define COLEDITDLG_H

#include <QDialog>
#include <QTableWidget>
#include "idatabasepp.hpp"
#include "framelesswindow.h"
#include "newcoldlg.h"

struct NewColData {
    GString colname;
    GString colfill;
    EDataType datatype;

    NewColData(const NewColData& cd)
    {
        colname = cd.colname;
        colfill = cd.colfill;
        datatype = cd.datatype;

    }
    NewColData() {
        colname = L"";
        colfill = L"";
        datatype = EDataType::e_Unknown;

    }
    NewColData(const NewColEvent& nce)
    {
        colname = nce.name.toStdWString();
        colfill = nce.fill.toStdWString();
        datatype = (EDataType)nce.dataType;


    }


};
namespace Ui {
class ColEditDlg;
}

class ColEditDlg : public QDialog
{
    Q_OBJECT

public:
    ColEditDlg(QWidget* parent,ZTable& tab,FramelessWindow* fwparent);
    explicit ColEditDlg(QWidget *parent = 0);
    ~ColEditDlg();
    ZTable m_Table;
    FramelessWindow* pFwParent;

    int exec() override;

public slots:
    void accept() override;

private slots:
    void on_btnAdd_clicked();
    
    void on_btnRemove_clicked();


private:
    QTableWidget* TableC;
    Ui::ColEditDlg *ui;
};

#endif // COLEDITDLG_H
