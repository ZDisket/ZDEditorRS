#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "generictablepage.h"
#include <thread>
#include <QKeyEvent>
#include "framelesswindow.h"

namespace Ui {
class SearchWindow;
}

struct Point{
    int col;
    int row;
    Point(){
        col = 0;
        row = 0;

    }

    Point(const int& icol,const int& irow){
        col = icol;
        row = irow;
    }
    Point(const Point& po){
        col = po.col;
        row = po.row;
    }
};
class SearchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = 0);
    ~SearchWindow();
    GenericTablePage* gtpCurrent;

    void keyPressEvent(QKeyEvent* pe) override;

    SearchWindow(QMainWindow *MaiWi, QString TableName, GenericTablePage *gtpIn, QWidget *inparent);
private slots:
    void on_btnSearch_clicked();

    void on_actionSEARCHHOTKEY_triggered();

private:
    typedef QMainWindow _super;


    Qt::CaseSensitivity lastCase;

    FramelessWindow* fwParent;
    // Pointer to the main window we use to check for valid tabs.
    QMainWindow* MWin;
    void ListenForKeys();
    std::thread* tKeyListener;
    Point LastHi;
    void Highlight(const Point& ptHigh);
    QString lSearch;
    QString lCol;

    QString CTableName;
    int ocuid;
    std::vector<Point> SearchRes;
    QLabel* staLbl;
    Ui::SearchWindow *ui;
};

#endif // SEARCHWINDOW_H
