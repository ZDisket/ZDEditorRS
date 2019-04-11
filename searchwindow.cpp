#include "searchwindow.h"
#include "ui_searchwindow.h"
#include "macros.hpp"
#include <QMessageBox>
#include <QShortcut>
#define STL(insta) staLbl->setText(insta)
#define _Q(str) QString(str)
#include "mainwindow.h"

#pragma comment(lib,"User32.lib")
SearchWindow::SearchWindow(QMainWindow* MaiWi,QString TableName,GenericTablePage *gtpIn, QWidget *inparent) : SearchWindow(inparent)
{

    gtpCurrent = gtpIn;

    ui->lblTableName->setText(TableName);

    for (int i = 0; i < gtpCurrent->pTable->columnCount();++i)
          ui->comboCol->addItem(gtpCurrent->pTable->horizontalHeaderItem(i)->text());

    MWin = MaiWi;

    CTableName = TableName;

    if (CTableName.contains(".gst"))
        ui->lblTableImg->setPixmap(QPixmap(":/treeicons/strtab24.png"));
}

SearchWindow::SearchWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);

    staLbl = new QLabel(this);

    staLbl->setText("Search ready");
       staLbl->setMinimumSize(QSize(125,12));

        // add the two controls to the status bar
        ui->statusbar->addPermanentWidget(staLbl);

    ocuid = 0;
    lSearch = "";
    lCol = "";


    fwParent = (FramelessWindow*)parent;
    ui->btnSearch->setDefault(true);

    tKeyListener = new std::thread(&SearchWindow::ListenForKeys,this);

    lastCase = Qt::CaseInsensitive;


}

SearchWindow::~SearchWindow()
{
    delete ui;
    delete staLbl;

    // Decolor all our previous markings.
    if (SearchRes.size() && ((MainWindow*)MWin)->findTab(CTableName) != -1)
    {


        MI(SearchRes,SeIt);
        while (SeIt != SearchRes.end()){
            Point& ptCu = *SeIt;

            gtpCurrent->pTable->item(ptCu.row,ptCu.col)->setBackgroundColor(QColor(0,0,0,0));


            ++SeIt;
        }
    }
}

void SearchWindow::keyPressEvent(QKeyEvent *pe)
{
    if(pe->key() == Qt::Key_Return)
        on_btnSearch_clicked();
    else
        _super::keyPressEvent(pe);
}

void SearchWindow::on_btnSearch_clicked()
{
    IF_ERROR(ui->edtSearch->text().isEmpty(),"Cannot search empty term");

    if (((MainWindow*)MWin)->findTab(CTableName) == -1)
    {
       QMessageBox(QMessageBox::Icon::Critical,"Error","Cannot find the tab that contains the targeted table. It may have been closed. Press OK to quit search.",QMessageBox::Ok,this).exec();
       fwParent->close();

       return;

    }

    QString csComboType = ui->comboCol->itemText(ui->comboCol->currentIndex());
    QString csSearchTxt = ui->edtSearch->text();

    Qt::CaseSensitivity Casei = Qt::CaseInsensitive;
    if (ui->chkCaseSensitive->checkState() == Qt::Checked)
        Casei = Qt::CaseSensitive;

    if (lSearch != csSearchTxt || lCol != csComboType || lastCase != Casei ){

        MI(SearchRes,SeIt);
        while (SeIt != SearchRes.end()){
            Point& ptCu = *SeIt;

            gtpCurrent->pTable->item(ptCu.row,ptCu.col)->setBackgroundColor(QColor(0,0,0,0));


            ++SeIt;
        }
        SearchRes.clear();


    }




    if (!SearchRes.size())
        ocuid = 0;


    // Find targeted column.

    int scolid = -1;
    for (int i = 0; i < gtpCurrent->pTable->columnCount();++i){
           if (csComboType == gtpCurrent->pTable->horizontalHeaderItem(i)->text())
               scolid = i;


    }



    IF_ERROR(scolid == -1,"Internal search error ocurred, can't match column. You shouldn't be seeing this.");



    if (!SearchRes.size())
    {
        // Clear the background color


        for (int nRow = 0;nRow < gtpCurrent->pTable->rowCount();++nRow){
            if (gtpCurrent->pTable->item(nRow,scolid)->text().contains(csSearchTxt,Casei)){
                Point Ptadd(scolid,nRow);
                SearchRes.push_back(Ptadd);
                QColor colyel(30,144,255);

                gtpCurrent->pTable->item(Ptadd.row,Ptadd.col)->setBackgroundColor(colyel);
            }


        }

        STL(_Q("Found (") + QString::number((int)SearchRes.size()) + _Q(")"));
        lSearch = csSearchTxt;
        lCol = csComboType;
        lastCase = Casei;

        ocuid = 0;
        if (SearchRes.size()){
            LastHi = SearchRes[0];
            Highlight(SearchRes[0]);

        }
        else{
            return;

        }





    }else
    {


        Highlight(SearchRes[ocuid]);
        if (ocuid < (int)SearchRes.size() - 1)
                ocuid++;
            else
                ocuid = 0;

        STL(_Q("Find (") + QString::number(ocuid) + _Q("/") + QString::number((int)SearchRes.size()) + _Q(")"));

    }



}

void SearchWindow::Highlight(const Point &ptHigh)
{
    // We remember and decolor the last highlight so we don't paint multiple items red.
    QColor colyel(30,144,255);
    gtpCurrent->pTable->item(LastHi.row,LastHi.col)->setBackgroundColor(colyel);


    QColor qtHigh(255,0,0);
    gtpCurrent->pTable->item(ptHigh.row,ptHigh.col)->setBackgroundColor(qtHigh);

    gtpCurrent->pTable->scrollToItem(gtpCurrent->pTable->item(ptHigh.row,ptHigh.col));

    LastHi = ptHigh;

    gtpCurrent->pTable->repaint();

}

void SearchWindow::on_actionSEARCHHOTKEY_triggered()
{
    // on_btnSearch_clicked();
}
#define KeyDown(vk) GetKeyState(vk) & 0x8000

void SearchWindow::ListenForKeys()
{

    // I'm too lazy to figure out QShortcut so I'll just do it the old fashioned WinAPI way.
  const bool t = true;
  while (t){

      Sleep(50);

  }


}
