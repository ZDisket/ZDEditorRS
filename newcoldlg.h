#ifndef NEWCOLDLG_H
#define NEWCOLDLG_H
#include <QDialog>

struct NewColEvent{
    QString name;
    QString fill;

    // in the enum
    unsigned short dataType;

};




namespace Ui {
class NewColDlg;
}

class NewColDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewColDlg(QWidget *parent = 0);
    ~NewColDlg();

    NewColEvent rEvent;

public slots:
    void accept() override;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewColDlg *ui;
};

#endif // NEWCOLDLG_H
