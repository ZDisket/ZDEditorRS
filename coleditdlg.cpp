#include "coleditdlg.h"
#include "ui_coleditdlg.h"
#include "macros.hpp"
const GString TypeStrings[] = { L"CHAR",    L"DECIMAL", L"DOUBLE PRECISION", L"FLOAT",   L"BIGINT", L"NUMERIC", L"SMALLINT",L"TIMESTAMP", L"VARCHAR" };

using namespace ZDB;
ColEditDlg::ColEditDlg(QWidget *inparent, ZTable &tab, FramelessWindow *fwparent) : ColEditDlg(inparent)
{
    m_Table = tab;
    pFwParent = fwparent;




}

ColEditDlg::ColEditDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColEditDlg)
{
    ui->setupUi(this);


}

ColEditDlg::~ColEditDlg()
{
    delete ui;
}

int ColEditDlg::exec()
{
    TableC = ui->tableCols;
    TableC->setRowCount((int)m_Table.Columns().size());
    TableC->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ColIt Itc = m_Table.Columns().begin();
    int d = 0;
    while (Itc != m_Table.Columns().end())
    {
        ZColumn& Col = *Itc;
        TableC->setItem(d,0,new QTableWidgetItem(QString::fromStdWString(TypeStrings[(int)Col.dataType])));
        TableC->setItem(d,1,new QTableWidgetItem(QString::fromStdWString(Col.name)));



        ++d;
        ++Itc;
    }

    pFwParent->resize(569,349);
    pFwParent->SetTitleBarBtns(false,false,true);

    return QDialog::exec();
}

void ColEditDlg::accept()
{
    pFwParent->close();
    QDialog::accept();
}

void ColEditDlg::on_btnAdd_clicked()
{

    FramelessWindow FDlg(this);
     FDlg.setWindowIcon(QIcon(":/acticons/acol64.png"));
     FDlg.setWindowTitle("New Column");
     FDlg.SetTitleBarBtns(false,false,true);
     NewColDlg dlg(this);

     FDlg.setContent(&dlg);
     FDlg.show();
     FDlg.resize(519,225);

     int nres = dlg.exec();

     if (nres != QDialog::Accepted)
         return;

     if (dlg.rEvent.dataType == 999)
         return;

         NewColData ncd(dlg.rEvent);
         ZColumn NCol(ncd.colname,m_Table.Columns().size(),ncd.datatype);
         // Fill the column

         if (m_Table.Columns().size())
         {
             if (m_Table.Columns()[0].Items().size()) {
                 NCol.vItems.reserve(m_Table.Columns()[0].vItems.size());
                 for (size_t x = 0; x < m_Table.Columns()[0].vItems.size(); x++)
                 {
                     ZItem NIt(ncd.colfill, x);

                     NCol.vItems.push_back(NIt);


                 }
             }

         }
         else {
             ZItem NIt(ncd.colfill, 0);

             NCol.vItems.push_back(NIt);


         }

         m_Table.Columns().push_back(NCol);

        TableC->insertRow(TableC->rowCount());
        const int ri = TableC->rowCount() - 1;
        QTableWidgetItem* dpItem = new QTableWidgetItem(QString::fromStdWString(TypeStrings[(short)ncd.datatype]));
        TableC->setItem(ri,0,dpItem);
        TableC->setItem(ri,1,new QTableWidgetItem(QString::fromStdWString(ncd.colname) ) );

        TableC->scrollToItem(dpItem);
        TableC->selectRow(ri);

}

void ColEditDlg::on_btnRemove_clicked()
{
    QList<QTableWidgetItem*> seli = TableC->selectedItems();
       QList<QTableWidgetItem*>::iterator It = seli.begin();
       while (It != seli.end())
       {
           QTableWidgetItem* item = *It;

           MI(m_Table.Columns(),CrIt);

           while (CrIt != m_Table.Columns().end())
           {
               if (item->text().toStdWString() == CrIt->name)
               {
                   m_Table.Columns().erase(CrIt);

                   break;
               }

               ++CrIt;
           }
           TableC->removeRow(item->row());

           ++It;
       }

}
