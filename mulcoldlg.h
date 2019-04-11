#ifndef MULCOLDLG_H
#define MULCOLDLG_H

#include <QDialog>
#include "framelesswindow.h"
#include <QSizeGrip>
namespace Ui {
class MulColDlg;
}

class MulColDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MulColDlg(QWidget *parent = 0);
    ~MulColDlg();

    void accept() override;
    int exec() override;

    // To be fetched by caller on result
    QString fill;
    int num;
    bool addrows;

    // To be filled by caller
    int hnum;

    FramelessWindow* pFwParent;

private slots:
    void on_spbCount_valueChanged(int arg1);

    void on_edtFill_textChanged(const QString &arg1);

private:
    QSizeGrip* SzGrip;
    Ui::MulColDlg *ui;
};

#endif // MULCOLDLG_H
