#include "generictablepage.h"
#include "ui_generictablepage.h"
#include "macros.hpp"
#include <QMessageBox>
#include <golem_string.h>
#include <limits.h>

GenericTablePage::GenericTablePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GenericTablePage)
{
    ui->setupUi(this);

    pTable = ui->tableW;
}

GenericTablePage::~GenericTablePage()
{
    delete ui;
}

void GenericTablePage::on_tableW_windowIconChanged(const QIcon &icon)
{
icon;
}

void GenericTablePage::on_tableW_itemChanged(QTableWidgetItem *item)
{

    if (!Eval(item->text().toStdWString(),mDBTable.Columns()[item->column()].dataType)){
        FindUndo(item);
    }else{
        Edit edtAdd;
        edtAdd.pos = make_pair(item->row(),item->column());
        edtAdd.txt = item->text();
        Edits.push_back(edtAdd);

    }


}
#define TRYCONV_ERR(call,errcaption) try{call;bret = true;}catch(const std::invalid_argument& ivex){QMessageBox(QMessageBox::Icon::Critical,"Input Error",QString(errcaption + QString(": ") + QString::fromLatin1(ivex.what(),strlen(ivex.what()))),QMessageBox::Ok,this).exec();bret = false;break;}

bool GenericTablePage::Eval(const GString &vstr, const EDataType &type)
{
    bool bret = false;
        switch(type)
        {
        case EDataType::e_Decimal: {
            try {
                stoll(vstr);
                bret = true;
            }
            catch (const std::invalid_argument& ivex) {
               QMessageBox(QMessageBox::Icon::Critical,"Error",QString("Cannot convert to 64 bit integer." + QString(": ") + QString::fromLatin1(ivex.what(),strlen(ivex.what()))),QMessageBox::Ok,this).exec();
                // Stop the compiler whining about unused variables with this useless function call.
                ivex.what();
            }

            break;
        }
        case EDataType::e_Double:{
            TRYCONV_ERR(stod(vstr), "Cannot convert to double precision float");
            break;
        }
        case EDataType::e_Float: {
            TRYCONV_ERR(stof(vstr), "Cannot convert to floating point number.");
            break;
        }
        case EDataType::e_Int: {
            TRYCONV_ERR(stoi(vstr), "Cannot convert to number");
            break;
        }
        case EDataType::e_Numeric:
        {
            TRYCONV_ERR(stod(vstr), "Cannot convert to numerical");
            break;
        }
        case EDataType::e_SmallInt:
        {
            int num = 0;
            TRYCONV_ERR(num = stoi(vstr), "Cannot convert to short int, invalid argument");

            if (num > SHRT_MAX) {

                bret = false;
                QMessageBox(QMessageBox::Icon::Critical,"Error",QString("Invalid input, number must be below " + QString::number(SHRT_MAX)),QMessageBox::Ok,this).exec();

            }

            break;
        }
        case EDataType::e_Timestamp:
        {
            GDateTime TimeP;
            bret = true;
            if (!TimeP.Parse(vstr)) {
                bret = false;
                QMessageBox(QMessageBox::Icon::Critical,"Error",QString("Input is not a valid date"),QMessageBox::Ok,this).exec();

            }
            break;
        }
        default:
            return true;


        }

        return bret;
}

void GenericTablePage::FindUndo(QTableWidgetItem *pItem)
{

    pTable->blockSignals(true);
    // 1 : Search in the undo list
    // 2: Search in the table
    // 3: Make it an empty one


    // 1
    vector<Edit>::iterator Eit = Edits.begin();
    while (Eit != Edits.end())
    {
        Edit& edt = *Eit;

        if (make_pair(pItem->row(),pItem->column()) == edt.pos){
            pItem->setText(edt.txt);
            pTable->blockSignals(false);

            return;
        }



        ++Eit;
    }


    if ((int)mDBTable.Columns()[pItem->column()].Items().size() > pItem->row()){
        pItem->setText(QString::fromStdWString(mDBTable.Columns()[pItem->column()].Items()[pItem->row()].sData));
        pTable->blockSignals(false);

        return;
    }

    pItem->setText("");


    pTable->blockSignals(false);


}
