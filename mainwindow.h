#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <qtreewidget.h>
#include <qtablewidget.h>
#include <idatabasepp.hpp>
#include <QLabel>
#include <QProgressBar>
#include "framelesswindow.h"
#include "StrTable/GStringTable.h"
#include "ext/ZCharScanner.h"
#include "ext/ZDFS.h"
#include <utility>
#include "searchwindow.h"

namespace ZTUtil{
template<typename Tp>
Tp IStrStreamConv(const std::wstring& Arg);

}
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    const char* args;

    void OpenFromArgs(const char* path);

    FramelessWindow* pParent;
    int findTab(const QString& name);

    void uprog(int in);
    void umax(int in);

    void ResetEntry(QTableWidgetItem* pItem);


private slots:
    void on_actionOpen_triggered();

    void on_treeTables_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_actionSave_triggered();

    void on_actionAdd_triggered();

    void on_actionDuplicate_triggered();

    void on_actionRemove_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionEdit_Columns_triggered();

    void on_actionSearch_triggered();

    void on_actionClear_triggered();

    void on_actionICSV_triggered();

    void on_actionECSV_triggered();

    void on_actionSave_As_triggered();

    void on_actVtable_triggered();

    void on_actAMulCol_triggered();

    void on_actionAssociate_file_format_triggered();

    void on_actGoToStrT_triggered();

private:
    DBManager Database;
    ZDFS FileSys;

    int maxstrid;

    pair<FramelessWindow*,SearchWindow*> pSearchWindow;
    ZTable ZTFromStrEntry(const GString& strename,vector<StrEntry>& strev);
    vector<StrEntry> StrEntryFromZT(ZTable & ztstr);
    GStringTable m_StringTable;
    // ID of the current TAB that holds the table.
    int ctid;

    void SetControls(bool ctrls);
    bool Associate();

    void CheckForAssoc();

    GString PDelim;

    QStringList nullh;

    void BuildDatabaseTree();

    QString CurrentFileName;
    QString CurrentFullFilePath;

    void LoadTable(ZTable &tab);

    void Save();

    std::vector<ZTable> OpenTables;

    int GetOpenTable(const GString& tbnam);
    bool GetTable(const GString& TableName,ZTable& table_out);

    GString ctName;

    ZTable& GetCurrentTable();



    void SaveCTChanges();
    bool loading;
    QTreeWidgetItem* addTreeRoot(QString name,QIcon ico);
    QTreeWidgetItem* addTreeChild(QTreeWidgetItem *parent,QString name,QIcon ico);
    QLabel* statusLabel;
    QProgressBar* statusProgressBar;

    QTreeWidget* treeUndo;

    void SError(const QString& caption,const QString& title = "Error" );
    Ui::MainWindow *ui;
    QTreeWidget* treeTables;
    QTableWidget* Table;
};

#endif // MAINWINDOW_H
