#include "newcoldlg.h"
#include "ui_newcoldlg.h"
#include "macros.hpp"
#include <QMessageBox>
#include <golem_string.h>

const GString TypeStrings[] = { L"CHAR",    L"DECIMAL", L"DOUBLE PRECISION", L"FLOAT",   L"BIGINT", L"NUMERIC", L"SMALLINT",L"TIMESTAMP", L"VARCHAR" };

NewColDlg::NewColDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewColDlg)
{
    ui->setupUi(this);
    rEvent.dataType = 999;
}

NewColDlg::~NewColDlg()
{
    delete ui;
}

void NewColDlg::accept()
{
    IF_ERROR(ui->edtFill->text().isEmpty(),"Fill is empty");
    IF_ERROR(ui->edtColumnName->text().isEmpty(),"Column name is empty.");
    IF_ERROR(ui->edtColumnName->text().contains(" ",Qt::CaseInsensitive),"Column name cannot contain spaces.");

    rEvent.dataType = 999;

    GString sDataTypeStr = ui->comboDType->currentText().toStdWString();
    for (unsigned short i = 0; i < 9; i++) {
        if (sDataTypeStr == TypeStrings[i])
            rEvent.dataType = i;

    }

    IF_ERROR(rEvent.dataType == 999,"CRITICAL ERROR: Failed to match data type. You shouldn't be seeing this error");
    rEvent.fill = ui->edtFill->text();
    rEvent.name = ui->edtColumnName->text();
    QDialog::accept();

}

void NewColDlg::on_buttonBox_accepted()
{



}
