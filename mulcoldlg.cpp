#include "mulcoldlg.h"
#include "ui_mulcoldlg.h"

MulColDlg::MulColDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MulColDlg)
{
    ui->setupUi(this);
    num = 0;
    fill = "0.000000";

}

MulColDlg::~MulColDlg()
{
    delete ui;
}

void MulColDlg::accept()
{
    num = ui->spbCount->value() + 1;

    pFwParent->close();
    addrows = ui->chkAddRows->isChecked();
    QDialog::accept();
}

int MulColDlg::exec()
{
    pFwParent->resize(332,331);

    pFwParent->SetTitleBarBtns(false,false,true);

    ui->redtSee->clear();
    for (int i = 1; i < 3;++i)
        ui->redtSee->append(QString("COUNTRY_") + QString::number(hnum + i) + '\n');

    return QDialog::exec();
}

void MulColDlg::on_spbCount_valueChanged(int arg1)
{
    num = arg1 + 1;
    // Preview
    ui->redtSee->clear();
    for (int i = 1; i < num;++i)
        ui->redtSee->append(QString("COUNTRY_") + QString::number(hnum + i) + '\n');


}

void MulColDlg::on_edtFill_textChanged(const QString &arg1)
{
    fill = arg1;
}
