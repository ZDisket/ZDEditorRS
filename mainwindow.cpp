#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framelesswindow.h"
#include "generictablepage.h"
#include "coleditdlg.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <fstream>
#include "macros.hpp"
#include "mulcoldlg.h"
#include <QFileInfo>
#include <sstream>

#include <vector>
using namespace std;
using namespace ZDB;

#define _Q(s) QString(s)
#define Status(sta) statusLabel->setText(sta)


QProgressBar* gpPgBar;
void pgMax(const int& in){
gpPgBar->setRange(0,in);
}
void PgPos(const int& pos){

if (gpPgBar->maximum() > pos )
    gpPgBar->setValue(pos);

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
loading = false;
/*
 What you see in mainwindow.ui is a lie. The sizes are adjusted here.
 The government is a lie. Life is a lie.
 EVERYTHING IS A LIE
*/
ui->splitter->setSizes(QList<int>() << 350 << 200);


    statusLabel = new QLabel(this);
           statusProgressBar = new QProgressBar(this);
    QLabel* versionLabel = new QLabel(this);
    gpPgBar = statusProgressBar;
    versionLabel->setText("V1.3A");
           // set text for the label
       statusLabel->setText("Loading to table..........");
          statusLabel->setMinimumSize(QSize(125,12));
           // make progress bar text invisible
           statusProgressBar->setTextVisible(false);
           // add the two controls to the status bar
           ui->statusBar->addPermanentWidget(statusLabel);
           ui->statusBar->addPermanentWidget(statusProgressBar,1);
           ui->statusBar->addPermanentWidget(versionLabel);


           Table = nullptr;

            ui->statusBar->showMessage("Ready.");
            statusLabel->setText("Loading to table..........");
           statusLabel->setText("Ready.");

           Database.SetPGMaxF(&pgMax);
           Database.SetPGPosF(&PgPos);
           treeTables = ui->treeTables;

           m_StringTable.UpdateProg.Connect(this,&MainWindow::uprog);
           m_StringTable.UpdateRangeMax.Connect(this,&MainWindow::umax);


          pSearchWindow = make_pair((FramelessWindow*)NULL,(SearchWindow*)NULL);


        CheckForAssoc();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenFromArgs(const char *path)
{

    if (!path)
        return;

    if (strlen(path) == 0)
        return;

    QString openfname(path);

        statusLabel->setText("Loading file");

        if (!Database.Init(openfname.toStdString())){
            SError("Cannot open database file");
            statusLabel->setText("Ready(error)");

            return;

        }
   //     Table->clear();


       // OpenTables.clear();

        //  Qt's file dialogs and stuff give us paths delimited by forward slash '/'
        // but command line arguments in Windows give us those delimited by backwards slash. That's why
        // the split command is different than the one at the open file dialog.
        QStringList lstPath = openfname.split("\\");
        CurrentFileName = lstPath.at(lstPath.size() - 1);
        CurrentFullFilePath = openfname;
        BuildDatabaseTree();


        statusLabel->setText("Ready");

        PDelim = L"\\";

}

void MainWindow::on_actionOpen_triggered()
{
    QString fnamex = QFileDialog::getOpenFileName(this, tr("Open Database"), QString(), tr("Database Files (*.gdb)"));


    if (fnamex == "")
        return;

    statusLabel->setText("Loading file");

    if (!Database.Init(fnamex.toStdString())){
        SError("Cannot open database file");
        statusLabel->setText("Ready(error)");

        return;

    }


    ui->tabWidget->clear();

    Table = nullptr;


    OpenTables.clear();
    QStringList lstPath = fnamex.split("/");
    CurrentFileName = lstPath.at(lstPath.size() - 1);
    CurrentFullFilePath = fnamex;
    BuildDatabaseTree();


    PDelim = L"/";
    statusLabel->setText("Ready");

}

void MainWindow::BuildDatabaseTree()
{
    treeTables->clear();

    QTreeWidgetItem* dbfile = addTreeRoot(CurrentFileName,QIcon(":/ico/treeicons/database64.png"));


    vector<GSString> tableNames = Database.TableNames();

    vector<GSString>::iterator It = tableNames.begin();
    while (It != tableNames.end())
    {
        addTreeChild(dbfile,QString::fromStdString(*It),QIcon(":/ico/treeicons/table64.png"));

        ++It;
    }

    dbfile->setExpanded(true);

    ZStringDelimiter DelimFindPath(CurrentFullFilePath.toStdWString());
    DelimFindPath.AddDelimiter(L"/");
    if (!DelimFindPath.szTokens())
        DelimFindPath.AddDelimiter(L"\\");

    // If we use a string delimiter to reassemble the full file path excluding the last token we should get the directory the GDB
    // is in so we can scan for gst files.
    GString gdbfolder = DelimFindPath.Reassemble(L"\\", (int)(DelimFindPath.szTokens() - 1));
    vector<SItemW> GstFiles = FileSys.StuffInDirectory(gdbfolder, L"*.gst");


    if (!GstFiles.size())
          return;

    QTreeWidgetItem* tiStrs = addTreeRoot("String Tables",QIcon(":/ico/treeicons/strtabroot64.png"));
    vector<SItemW>::iterator Its = GstFiles.begin();
    while (Its != GstFiles.end())
    {

        addTreeChild(tiStrs,QString::fromStdWString(Its->Name),QIcon(":/ico/treeicons/strtabico64.png"));


        ++Its;
    }
    tiStrs->setExpanded(true);

}

void MainWindow::LoadTable(ZTable &tab)
{
   Table->blockSignals(true);
    ui->menuTable->setEnabled(true);
    nullh.clear();
    loading = true;
    Table->clear();
    statusLabel->setText("Preparing columns");
    Table->setColumnCount((int)tab.Columns().size());

    QStringList ColLabels;

    for (size_t c = 0;c < tab.Columns().size();++c)
        ColLabels << QString::fromStdWString(tab.Columns()[c].name);


    Table->setHorizontalHeaderLabels(ColLabels);
    Table->horizontalHeader()->setSectionsMovable(true);


    statusLabel->setText("Building table");
    const int rc = (int)tab.GetNItems() / tab.Columns().size();
    Table->setRowCount(rc);
    nullh.reserve(rc);
    statusProgressBar->setRange(0,(int) tab.Columns()[0].vItems.size() * tab.Columns().size());
    int prog = 0;
    ColIt Itc = tab.Columns().begin();
    int colid = 0;
    bool appheader = (tab.GetName().find(L".gst") == GString::npos);
    while (Itc != tab.Columns().end())
    {
        int itemid = 0;
        ZItemIt Iti = Itc->vItems.begin();

        while (Iti != Itc->vItems.end())
        {

              //  statusProgressBar->setValue(prog);

            Table->setItem(itemid,colid,new QTableWidgetItem(QString::fromStdWString(Iti->sData)));

            if (colid == 0 && appheader)
                nullh << " > ";


            ++prog;
            ++itemid;
            ++Iti;
        }
        ++colid;
        ++Itc;
    }
    statusProgressBar->setValue(0);


    if (tab.GetName().find(L".gst") == GString::npos)
        Table->setVerticalHeaderLabels(nullh);
    else
        Table->setColumnWidth(1,(int)Table->size().width() * 0.9);


    statusLabel->setText("Ready");
    loading = false;

    ctName = tab.GetName();

    if (GetOpenTable(ctName) == -1)
        OpenTables.push_back(tab);

    Table->blockSignals(false);


}

void MainWindow::Save()
{
    // Since we are saving to an already existing file, we only need to save the tables that we modified.

    try {
        SaveCTChanges();
        vector<ZTable>::iterator ItA = OpenTables.begin();
        int i = 0;
        GString sta = L"Saving table " + GString(i) + L"/" + GString((int)OpenTables.size());
        statusLabel->setText(QString::fromStdWString(sta));
        while (ItA != OpenTables.end())
        {
            ++i;

            sta = L"Saving table " + GString(i) + L"/" + GString((int)OpenTables.size());

            statusLabel->setText(QString::fromStdWString(sta));

            DBError::DBError Err;
            if (ItA->GetName().find(L".gst") == GString::npos) {

            if (!ItA->Columns().size()){
                ++ItA;
                continue;

            }


                    try {
                    Err = Database.Save(*ItA);

                    }
                    catch(...) {
                        int r = QMessageBox::critical(this,"Error","Something went wrong while saving.\nWould you like to try again?",QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No);
                        if (r == QMessageBox::StandardButton::Yes)
                            Save();
                        else
                            break;


                    }


                    if (Err != DBError::err_no_error)
                        SError("Failed to save with error " + QString::number((int)Err));
            }
            else {

                m_StringTable.Data() = StrEntryFromZT(*ItA);
                ZStringDelimiter DelimFindPath(CurrentFullFilePath.toStdWString());
                DelimFindPath.AddDelimiter(PDelim);
                // If we use a string delimiter to reassemble the full file path excluding the last token we should get the directory the GDB
                // is in so we can add the file name and get the table.
                GString gstfile = DelimFindPath.Reassemble(L"\\", (int)(DelimFindPath.szTokens() - 1)) + L"\\" + ItA->GetName();
                Status("Saving string table");
             bool strs = m_StringTable.Save(gstfile);
             if (!strs)
                 SError("Failed to save string table");
            }

        ++ItA;
        }
        statusProgressBar->setValue(0);
        statusLabel->setText("Ready");

    }
    catch(...) {
        int r = QMessageBox::critical(this,"Error","Something went wrong while saving.\nWould you like to try again?",QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No);
        if (r == QMessageBox::StandardButton::Yes)
            Save();
        else
            return;


    }

}

int MainWindow::GetOpenTable(const GString &tname)
{
    if (!OpenTables.size())
            return -1;

        vector<ZTable>::iterator It = OpenTables.begin();
        int r = 0;
        while (It != OpenTables.end())
        {
            if (It->GetName() == tname)
                return r;

            ++r;
            ++It;
        }

        // If we don't return up there, it means we got here, we haven't got anything.
        return -1;
}

bool MainWindow::GetTable(const GString &TableName, ZTable &table_out)
{
    // Check if this table is open, otherwise we just ask the database for it.
        GString tbnam = TableName;



        size_t startpos = TableName.find_first_of((wchar_t)0x20);
        if (GString::npos != startpos && TableName.find(L".gst") == GString::npos)
        {
            tbnam = TableName.substr(0, startpos);
        }
        int oti = GetOpenTable(tbnam);
        if (oti != -1) {
            // set the table
            const int fres = findTab(QString::fromStdWString(tbnam));
            if (fres != -1){
                GenericTablePage* ptp = (GenericTablePage*)ui->tabWidget->widget(findTab(QString::fromStdWString(tbnam) ));
                ui->tabWidget->setCurrentIndex(findTab(QString::fromStdWString(tbnam)));
                Table = ptp->pTable;

            }else{
                GenericTablePage* npage = new GenericTablePage(this);
                const int dex = ui->tabWidget->addTab(npage,QIcon(":/ico/treeicons/table64.png"),QString::fromStdWString(tbnam));
                GenericTablePage* ptp = (GenericTablePage*)ui->tabWidget->widget(dex);
                ui->tabWidget->setCurrentIndex(dex);
                Table = ptp->pTable;
                npage->pDBTable = &OpenTables[oti];
                npage->mDBTable = OpenTables[oti];


            }

            table_out = OpenTables[oti];
            return true;
        }
        //table_out.SetName(tbnam);

        if (tbnam.find(L".gst") != wstring::npos)
        {
            Status("Loading string table");
            ZStringDelimiter DelimFindPath(CurrentFullFilePath.toStdWString());
            DelimFindPath.AddDelimiter(L"/");

            // Just in case
            if (!DelimFindPath.szTokens())
                DelimFindPath.AddDelimiter(L"\\");

            // If we use a string delimiter to reassemble the full file path excluding the last token we should get the directory the GDB
            // is in so we can add the file name and get the table.
            GString gstfile = DelimFindPath.Reassemble(L"\\", (int)(DelimFindPath.szTokens() - 1) ) + L"\\" + tbnam;
            m_StringTable.Load(gstfile);

            table_out = ZTFromStrEntry(tbnam,m_StringTable.Data());

            GenericTablePage* npage = new GenericTablePage(this);
            const int dex = ui->tabWidget->addTab(npage,QIcon(":/ico/treeicons/strtabico64.png"),QString::fromStdWString(tbnam));
            GenericTablePage* ptp = (GenericTablePage*)ui->tabWidget->widget(dex);

            ui->tabWidget->setCurrentIndex(dex);

            npage->pDBTable = &table_out;
            npage->mDBTable = table_out;

            Table = ptp->pTable;

            return true;



        }


        // Add a new tab page if we haven't found it in the open tables.
        table_out = Database.GetZTable(GSString(TableName));
        GenericTablePage* npage = new GenericTablePage(this);
        const int dex = ui->tabWidget->addTab(npage,QIcon(":/ico/treeicons/table64.png"),QString::fromStdWString(tbnam));
        GenericTablePage* ptp = (GenericTablePage*)ui->tabWidget->widget(dex);
        ui->tabWidget->setCurrentIndex(dex);
        Table = ptp->pTable;
        ptp->pDBTable = &table_out;
        ptp->mDBTable = table_out;

        return true;
}

ZTable &MainWindow::GetCurrentTable()
{
    qDebug() << "getcurrenttable get table named " << QString::fromStdWString(OpenTables[GetOpenTable(ctName)].GetName());
    return OpenTables[GetOpenTable(ctName)];
}

void MainWindow::uprog(int in)
{
    statusProgressBar->setValue(in);
}

void MainWindow::umax(int in)
{
    statusProgressBar->setRange(0,in);
}

void MainWindow::ResetEntry(QTableWidgetItem *pItem)
{

}

void MainWindow::SaveCTChanges()
{
    if (OpenTables.empty())
        return;

    if (Table == nullptr)
        return;

    statusProgressBar->setRange(0,Table->columnCount() * Table->rowCount());
    statusLabel->setText("Caching table");
    // Assemble the whole table from the widget

    qDebug() << QString::fromStdWString(ctName);
    ZTable asmt;
    asmt.SetName(ctName,false);
    int pval = 0;
    const int rows = Table->rowCount();
    for (int col = 0;col < Table->columnCount();++col)
    {

        ZColumn ACol(Table->horizontalHeaderItem(col)->text().toStdWString(),(size_t)col,GetCurrentTable().Columns()[col].dataType);

        ACol.vItems.reserve(rows);
        for (int row = 0; row < rows;++row)
        {
            // statusProgressBar->setValue(pval);
            QTableWidgetItem* pItem = Table->item(row,col);

            std::wstring MStr = L"";
            if (pItem){
                MStr = pItem->text().toStdWString();

            }
            else{
                SError("Empty item at row " + QString::number(row) + _Q(" col: ") + QString::number(col));
                return;
            }



            if (ACol.dataType == EDataType::e_Double){
                double num = ZTUtil::IStrStreamConv<double>(MStr);

                wostringstream ss;
                ss << num;

                MStr = ss.str();
            }

            if (ACol.dataType == EDataType::e_Decimal || ACol.dataType == EDataType::e_Numeric){
                INT64 num = ZTUtil::IStrStreamConv<INT64>(MStr);


                wostringstream ss;
                ss << num;

                MStr = ss.str();

            }

            if (ACol.dataType == EDataType::e_Float){

                float num = ZTUtil::IStrStreamConv<float>(MStr);

                wostringstream ss;
                ss << num;

                MStr = ss.str();

            }


            ZItem addit(MStr,(size_t)row);
            ACol.vItems.push_back(addit);
            ++pval;

        }

        asmt.Columns().push_back(ACol);
    }
    const int ftres = findTab(QString::fromStdWString(ctName));

    if (ftres != -1){
        GenericTablePage* gtp = (GenericTablePage*)ui->tabWidget->widget(ftres);
        gtp->mDBTable = asmt;

    }
    GetCurrentTable() = asmt;

    statusProgressBar->setValue(0);
    statusLabel->setText("Ready");
}

QTreeWidgetItem *MainWindow::addTreeRoot(QString name, QIcon ico)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(treeTables);
    treeItem->setText(0,name);
    treeItem->setIcon(0,ico);

    return treeItem;
}

QTreeWidgetItem *MainWindow::addTreeChild(QTreeWidgetItem *parent, QString name, QIcon ico)
{

    QTreeWidgetItem *treeItemc = new QTreeWidgetItem();
    treeItemc->setText(0,name.trimmed());
    treeItemc->setIcon(0,ico);

    parent->addChild(treeItemc);

    return treeItemc;
}

void MainWindow::SError(const QString &caption, const QString &title)
{
    //FramelessWindow fw;

  //  QIcon icon(":/icons/res/zico.ico");
   //framelessWindow.setWindowIcon(icon);
    //fw.setWindowTitle(title);

    QMessageBox msgBox;
  //  msgBox.setStyle(ui->centralWidget->style());
    msgBox.setWindowTitle(title);
    msgBox.setText(caption);
    msgBox.setIcon(QMessageBox::Critical);

    msgBox.exec();

}

void MainWindow::on_treeTables_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // Prevent the event from being triggered while it's loading, causing crashs;
    treeTables->blockSignals(true);
    if (loading)
        return;


    if (item->childCount() == 0)
    {
        loading = true;
        ZTable gTab;

        GetTable(item->text(column).toStdString(),gTab);
        LoadTable(gTab);
        loading = false;

    }
    treeTables->blockSignals(false);


}

void MainWindow::on_actionSave_triggered()
{
    Save();
}

void MainWindow::on_actionAdd_triggered()
{
    Table->blockSignals(true);

    Table->insertRow(Table->rowCount());
    nullh << " > ";
    Table->setVerticalHeaderLabels(nullh);
   QTableWidgetItem* dpItem = Table->item(Table->rowCount() - 1,0);
   Table->scrollToItem(dpItem);
   Table->selectRow(Table->rowCount() - 1);

   if (GetCurrentTable().GetName().find(L".gst") != GString::npos){
       Table->setItem(Table->rowCount() - 1,0,new QTableWidgetItem(QString::number(maxstrid + 1)));
       ++maxstrid;
   }



   Table->blockSignals(false);


}

void MainWindow::on_actionDuplicate_triggered()
{
    QList<QTableWidgetItem*> seli = Table->selectedItems();

    if (!seli.size())
        return;

     Table->insertRow(Table->rowCount());
     const int at = Table->rowCount() - 1;

     for (int acol = 0;acol < Table->columnCount();++acol)
     {

         // Set across all columns our item.
         Table->setItem(at,acol,new QTableWidgetItem(*Table->item(seli[0]->row(),acol)));

     }

     QTableWidgetItem* dpItem = Table->item(at,0);
     Table->scrollToItem(dpItem);
     Table->selectRow(at);

     nullh << " > ";
     Table->setVerticalHeaderLabels(nullh);


}

void MainWindow::on_actionRemove_triggered()
{
    QList<QTableWidgetItem*> seli = Table->selectedItems();
       QList<QTableWidgetItem*>::iterator It = seli.begin();
       while (It != seli.end())
       {
           QTableWidgetItem* item = *It;
           Table->removeRow(item->row());

           ++It;
       }

}

int MainWindow::findTab(const QString &name)
{
    for (int i = 0;i < ui->tabWidget->count();++i){
        if (name == ui->tabWidget->tabText(i))
            return i;

    }
    return -1;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    GenericTablePage* ptp = (GenericTablePage*)ui->tabWidget->widget(index);

    if (!ptp)
        return;

    SaveCTChanges();
    // Set the table we're editing.

    ctName = ui->tabWidget->tabText(index).toStdWString();
    Table = ptp->pTable;

    if (ui->tabWidget->tabText(index).contains(".gst"))
        ui->tabWidget->setTabIcon(index,QIcon(":/ico/treeicons/strtabico64.png"));

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    GenericTablePage* gtpDestroy = (GenericTablePage*)ui->tabWidget->widget(index);

    ui->tabWidget->removeTab(index);
    delete gtpDestroy;
    gtpDestroy = nullptr;

    if (ui->tabWidget->count() == 0)
        ui->menuTable->setEnabled(false);

    Table = nullptr;
}

ZTable MainWindow::ZTFromStrEntry(const GString &strename, vector<StrEntry> &strev)
{
    Status("Prepare strings");
    std::vector<StrEntry>::iterator Set = strev.begin();
    ZTable ZTStringTab;
    ZTStringTab.SetName(strename,false);
    ZColumn ColID(L"ID", 0, EDataType::e_Int);
    ZColumn ColStr(L"String", 1, EDataType::e_Varchar);

    ZTStringTab.Columns().push_back(ColID);
    ZTStringTab.Columns().push_back(ColStr);

    ZTStringTab.Columns()[0].Items().reserve(strev.size());
    ZTStringTab.Columns()[1].Items().reserve(strev.size());

    statusProgressBar->setRange(0, (int)strev.size());
    size_t rid = 0;
    int lmid = 0;
    while (Set != strev.end())
    {
        ZItem ItID(GString(Set->ID), rid);
        ZItem ItStr(Set->GetString(), rid);
        ZTStringTab.Columns()[0].Items().push_back(ItID);
        ZTStringTab.Columns()[1].Items().push_back(ItStr);
  //      statusProgressBar->setValue((int)rid);


        if (Set->ID > lmid)
            lmid = Set->ID;

        ++rid;
        ++Set;
    }

    maxstrid = lmid;
    return ZTStringTab;
}

vector<StrEntry> MainWindow::StrEntryFromZT(ZTable &ztstr)
{
    Status("Pre-string saving");

    vector<StrEntry> Strer;
    Strer.reserve(ztstr.Columns()[0].Items().size());

    MI(ztstr.Columns()[0].Items(), CSit);
    size_t x = 0;
    statusProgressBar->setRange(0, (int)ztstr.Columns()[0].Items().size());
    while (CSit != ztstr.Columns()[0].Items().end())
    {

        StrEntry EnA(CSit->sData.ToINT32(), ztstr.Columns()[1].Items()[x].sData);
        Strer.push_back(EnA);
     //   statusProgressBar->setValue((int)x);
        ++x;
        ++CSit;
    }
    return Strer;
}

void MainWindow::SetControls(bool ctrls)
{
    QList<QWidget *> buttonsList = this->findChildren<QWidget *>();
        for (int i = 0; i < buttonsList.count(); i++){
            buttonsList.at(i)->setEnabled(ctrls);
        }
}
#pragma comment(lib,"Advapi32.lib")

BOOL IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}
bool MainWindow::Associate()
{
    if (!IsElevated()){
        SError("Cannot associate: Need administrator privileges.\nPlease restart as administrator and try again.");
        return false;
    }

    QString exepath = QCoreApplication::applicationDirPath();
       exepath.replace("/","\\");

       QString exef = QCoreApplication::applicationFilePath();
       exef.replace("/","\\");

    // Make the assoc command
    string asocmd = "assoc .gdb=zrsgdbf";
    // Make the ftype command which makes it send it to this program with the first argument
    string ftypecmd = string("ftype zrsgdbf=") + SQUOTE + exef.toStdString() + SQUOTE + " " + SQUOTE + "%1" + SQUOTE;

    // Make a registry add command to set that file's default icon to be the one we've got here
    string fticocmd = "reg add hkcr\\zrsgdbf\\DefaultIcon /f /ve /d " + std::string(SQUOTE) + exepath.toStdString() + "\\dbf.ico" + SQUOTE;



   system(asocmd.c_str());
   system(ftypecmd.c_str());
  system(fticocmd.c_str());

  std::ofstream msglog;
  msglog.open("reglog.txt");
  msglog << asocmd << '\n';
  msglog << ftypecmd << '\n';
  msglog << fticocmd << '\n';
  msglog.close();



 std::ofstream ofsAs;
 ofsAs.open(QString(exepath + "\\assoc.bin").toStdWString());
 ofsAs << "1";
 ofsAs.close();

  return true;

}
bool fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}
void MainWindow::CheckForAssoc()
{
    QString exepath = QCoreApplication::applicationDirPath();
       exepath.replace("/","\\");

       QString fpath = QString(exepath + "\\assoc.bin");

    if (fileExists(fpath))
        return;


    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Associate with files", "ZDEditorRS does not appear to be currently associated with the .gdb file format.\n Would you like to do that now?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          Associate();
      } else {
          std::ofstream ofsAs;
          ofsAs.open(fpath.toStdWString());
          ofsAs << "1";
          ofsAs.close();
      }




}

void MainWindow::on_actionEdit_Columns_triggered()
{

    if (!OpenTables.size())
        return;

    IF_ERROR(GetCurrentTable().GetName().find(L".gst") != GString::npos,"Cannot edit the columns of a string table.");
    SetControls(false);


    // Set our editing
        FramelessWindow FDlg(this);
         FDlg.setWindowIcon(QIcon(":/acticons/coledit64.png"));
         FDlg.setWindowTitle("Edit Columns");
         ColEditDlg dlg(this);
         dlg.m_Table = GetCurrentTable();
         dlg.pFwParent = &FDlg;

         FDlg.setContent(&dlg);

         FDlg.ContentDlg(&dlg);
         FDlg.show();
         // block
        dlg.setModal(true);
         if (dlg.exec() != QDialog::Accepted){
             SetControls(true);

             return;

         }
         SetControls(true);

         GetCurrentTable() = dlg.m_Table;
         LoadTable(dlg.m_Table);

         const int ftres = findTab(QString::fromStdWString(dlg.m_Table.GetName()));
         if (ftres != -1){
             GenericTablePage* gtp = (GenericTablePage*)ui->tabWidget->widget(ftres);
             gtp->mDBTable = dlg.m_Table;

         }

         return;


}

void MainWindow::on_actionSearch_triggered()
{

    AIF_RETURN(!OpenTables.size());

    QString Tname = QString::fromStdWString(GetCurrentTable().GetName());

    // FTR: Find Table Result
    const int ftr = findTab(Tname);
    AIF_RETURN(ftr == -1);


/*
    if (get<0>(pSearchWindow) != NULL){

        FramelessWindow* pFw = get<0>(pSearchWindow);

        if (pFw->isVisible())
            pFw->close();

        pSearchWindow = make_pair((FramelessWindow*)NULL,(SearchWindow*)NULL);

    };

*/
    // We create these windows with new, as this function doesn't block
    // and exits immediately, destroying them if they're allocated on the stack.

    FramelessWindow* FDlg = new FramelessWindow(this);
     FDlg->setWindowIcon(QIcon(":/acticons/zoom64.png"));
     FDlg->setWindowTitle("Search");
    SearchWindow* swin = new SearchWindow(this,Tname,(GenericTablePage*)ui->tabWidget->widget(ftr),FDlg);

    FDlg->SetTitleBarBtns(false,false,true);
     FDlg->setContent(swin);
     FDlg->resize(701,156);

     FDlg->setAttribute(Qt::WA_DeleteOnClose);
     swin->setAttribute(Qt::WA_DeleteOnClose);
     pSearchWindow = make_pair(FDlg,swin);
     FDlg->show();

}

void MainWindow::on_actionClear_triggered()
{

    Table->setRowCount(0);
}

void MainWindow::on_actionICSV_triggered()
{
    QString fnamescv = QFileDialog::getOpenFileName(this, tr("Open CSV to import"), QString(), tr("Comma Separated Variable Files (*.csv)"));

    if (fnamescv == "")
        return;


    ifstream inCSV(fnamescv.toStdWString());
    vector<GString> inLines;

    string out_read;

    if (inCSV.good()) {
        Status("Opening file");

        while (!inCSV.eof()) {
            getline(inCSV, out_read);
            inLines.push_back(out_read);

        }
    }
    else {
        SError("Cannot begin import, opening file failed");

        inCSV.close();

        return;
    }
    inCSV.close();

    // Check the first element
        ZStringDelimiter DelimLine(inLines[0]);
        DelimLine.AddDelimiter(L",");


        if (DelimLine.szTokens() != (size_t)Table->columnCount())
        {
            GString msg(L"Cannot import from this file, it is invalid. Expected " + GString(Table->columnCount()) + L" columns, got " + GString((int)DelimLine.szTokens()) + L" cols");

            SError(QString::fromStdWString(msg));

            Status("Ready (error)");
            return;
        }
        Status("Importing");

        statusProgressBar->setRange(0, (int)inLines.size());

      vector<GString>::iterator lIt = inLines.begin();

      //Pre-check the file for empty lines
      int fullLines = 0;
      while (lIt != inLines.end())
   {

      if (!lIt->empty())
          ++fullLines;

      ++lIt;
      }

      Table->setRowCount(fullLines);

      int l = 0;
      nullh.clear();

        nullh.reserve(fullLines);

        // Reset the iterator
      lIt = inLines.begin();

        Table->blockSignals(true);
        while (lIt != inLines.end())
     {
        GString& Line = *lIt;
        if (Line.empty()){
            ++lIt;
            continue;
        }

       ZStringDelimiter dLine(Line);
        dLine.AddDelimiter(L",");

        if (dLine.szTokens() != (size_t)Table->columnCount())
        {
            GString msg(L"Could only partially import from this file, it is invalid. Expected " + GString(Table->columnCount()) + L" columns, got " + GString((int)DelimLine.szTokens()) + L" cols at line " + GString((int)l));

            SError(QString::fromStdWString(msg));

            Status("Ready (error)");
            break;
        }
              // Since we verified that they're equal, the vector subscripts should be in range.
              const vector<GString>& vrTokens = dLine.GetTokens();
              for (size_t i = 0; i < vrTokens.size();++i)
              {

                  Table->setItem(l,i,new QTableWidgetItem(QString::fromStdWString(vrTokens[i])));

              }

              nullh << " > ";

              statusProgressBar->setValue(l);
            ++l;
            ++lIt;

     }
    Table->setVerticalHeaderLabels(nullh);

     statusProgressBar->setValue(0);
     Table->blockSignals(false);

     Status("Ready");


}

void MainWindow::on_actionECSV_triggered()
{
    QString ofnamescv = QFileDialog::getSaveFileName(this, tr("Export CSV"), QString::fromStdWString(GetCurrentTable().GetName()), tr("Comma Separated Variable Files (*.csv)"));
    if (ofnamescv == "")
        return;

    Status("Preparing exports");

    vector<GString> vFileLines;
    ZTable& Ctab = GetCurrentTable();

    // Easier to export with rows.
    vector<ZRow> Rows = Ctab.MakeRows();


    Status("Building file");
    statusProgressBar->setRange(0, (int)Rows.size());
    vector<ZRow>::iterator Rit = Rows.begin();
    int v = 0;
    while (Rit != Rows.end())
    {
        GString CuLine;
        ZRow& Row = *Rit;

        vector<ZRowItem>::iterator ItemIt = Row.Cells().begin();
        short index = 0;
        const short maxindex = (short)Row.Cells().size() - 1;
        while (ItemIt != Row.Cells().end())
        {

            ZRowItem& Item = *ItemIt;
            CuLine.append(Item.sData);

            if (index != maxindex)
                CuLine.append(L",");


            ++index;
            ++ItemIt;
        }
        vFileLines.push_back(CuLine);
        statusProgressBar->setValue(v);
        ++v;

        ++Rit;
    }
    Status("Exporting file.");
    statusProgressBar->setValue(1);
    statusProgressBar->setRange(0, (int)vFileLines.size());
    ofstream ofCSV(ofnamescv.toStdWString());

    for (size_t i = 0; i < vFileLines.size();++i)
    {
        statusProgressBar->setValue(i);
        ofCSV << GSString(vFileLines[i]) << "\n";


    }
    ofCSV.close();
     statusProgressBar->setValue(0);
Status("Ready");


}

void MainWindow::on_actionSave_As_triggered()
{
    QString sfname = QFileDialog::getSaveFileName(this, tr("Save Database"), QString(), tr("Database Files (*.gdb)"));


   // AStruct:
   // 1_ copy file to same dir (fname_T1)
   // 2 save current orig file
   // 3

    if (sfname == "")
        return;

    Status("Copying...");

    Database.Shutdown();


    if (CopyFile(CurrentFullFilePath.toStdWString().c_str(), sfname.toStdWString().c_str(), FALSE) == 0) {
        wchar_t err[256];
        memset(err, 0, 256);
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
            NULL, err, 255, NULL);

        GString cap(L"Could not begin saving file, copy failed: " + GString(err));
        SError(QString::fromStdWString(cap));

        Database.Init(CurrentFullFilePath.toStdString());
        return;

    }

    ZStringDelimiter DelimPath(sfname.toStdWString());
    DelimPath.AddDelimiter(PDelim);

    if (!DelimPath.szTokens())
        DelimPath.AddDelimiter(L"/");

    CurrentFullFilePath = QString::fromStdWString(DelimPath.Reassemble(L"\\"));


if (!Database.Init(CurrentFullFilePath.toStdString())){
    SError("Failed to save, file corrupted. Execution cannot continue","Fatal error");
    exit(-1);
  }




    CurrentFileName = QString::fromStdWString(DelimPath[DelimPath.szTokens() - 1]);

    Status("Now saving");
    Save();

    BuildDatabaseTree();

}
const GString TypeStrings[] = { L"CHAR",    L"DECIMAL", L"DOUBLE PRECISION", L"FLOAT",   L"BIGINT", L"NUMERIC", L"SMALLINT",L"TIMESTAMP", L"VARCHAR" };

void MainWindow::on_actVtable_triggered()
{
    if (OpenTables.empty())
        return;

    if (Table == NULL)
        return;


    IF_ERROR(GetCurrentTable().GetName().find(L".gst") != GString::npos,"Cannot verify a string table.");

    statusProgressBar->setRange(0,Table->columnCount() * Table->rowCount());
    statusLabel->setText("Verifying table");
    // Assemble the whole table from the widget

    qDebug() << QString::fromStdWString(ctName);
    ZTable asmt;
    asmt.SetName(ctName,false);
    int errors = 0;
    int pv = 0;
    const int rows = Table->rowCount();
    const int cols = Table->columnCount();

    std::vector<std::string> FileLines;

    QColor qtHigh(255,0,0);

    // Find all the errors

    Table->blockSignals(true);
    for (int col = 0; col < Table->columnCount();++col)
    {
        for (int row = 0; row < rows;++row)
        {
            ZColumn ACol(Table->horizontalHeaderItem(col)->text().toStdWString(),(size_t)col,GetCurrentTable().Columns()[col].dataType);

            // statusProgressBar->setValue(pval);
            QTableWidgetItem* pItem = Table->item(row,col);
            if (!pItem)
                continue;

            std::wstring MStr = L"";
            if (pItem){
                MStr = pItem->text().toStdWString();

            }
            else{
                QString err = "Empty item at row " + QString::number(row) + _Q(" col: ") + QString::number(col);
                FileLines.push_back(err.toStdString());
                pItem->setBackgroundColor(qtHigh);

                ++errors;

            }

            try {
                if (ACol.dataType == EDataType::e_Double){
                    double num = ZTUtil::IStrStreamConv<double>(MStr);

                    wostringstream ss;
                    ss << num;

                    MStr = ss.str();
                }

                if (ACol.dataType == EDataType::e_Decimal || ACol.dataType == EDataType::e_Numeric){
                    INT64 num = ZTUtil::IStrStreamConv<INT64>(MStr);


                    wostringstream ss;
                    ss << num;

                    MStr = ss.str();

                }

                if (ACol.dataType == EDataType::e_Float){

                    float num = ZTUtil::IStrStreamConv<float>(MStr);

                    wostringstream ss;
                    ss << num;

                    MStr = ss.str();

                }
            } catch (...) {

                QString err = "Invalid " + QString::fromStdWString(TypeStrings[(size_t)ACol.dataType]) + " at row: " + QString::number(row) + _Q(" col: ") + QString::number(col);
                FileLines.push_back(err.toStdString());
               pItem->setBackgroundColor(qtHigh);
               ++errors;

            }

            ++pv;
            statusProgressBar->setValue(pv);




        }
    }
    Table->blockSignals(false);

    statusProgressBar->setValue(0);

    if (errors == 0){
        QMessageBox::information(this,"Verification results","No errors found in this table.");
        statusLabel->setText("Ready.");

        return;
    }
    QMessageBox::critical(this,"Verification results",QString::number(errors) + " errors were found in the table. After pressing OK, you'll be prompted to save the results.");

    Status("Waiting");
    QString ofnlog = QFileDialog::getSaveFileName(this, tr("Save the log file"), QString::fromStdWString(GetCurrentTable().GetName() + L"_VLOG"), tr("Text files (*.txt)"));
    if (!ofnlog.size())
        return;

    statusLabel->setText("Exporting log file");

    std::ofstream ofsLog;
    ofsLog.open(ofnlog.toStdWString());
    auto Oit = FileLines.begin();
    while (Oit != FileLines.end())
    {
        ofsLog << *Oit << '\n';

        ++Oit;
    }

    ofsLog.close();

    QMessageBox::information(this,"Verification results","Log file was exported.");

    statusLabel->setText("Ready.");



}

void MainWindow::on_actAMulCol_triggered()
{
    if (!Table)
        return;

    if (GetCurrentTable().GetName().find(L"RELATIONS") == GString::npos)
    {
        SError("This does not appear to be a RELATIONS table");
        return;

    }
    // must be +1
    // Find the highest number

    const int Cols = Table->columnCount();
    int highest = 0;
    for (int c = 0; c < Cols; ++c)
    {
       QString lbl = Table->horizontalHeaderItem(c)->text();

       //Debug-in-release messagebox

       if (c > 1){
         //  SError(lbl);

           //SError(QString::fromStdString(lbl.toStdString().substr(8)));
          int num = stoi(lbl.toStdString().substr(8));
          if (num > highest)
              highest = num;
       }








    }

    // Invoke the dialog
    SetControls(false);


    // Set our editing
        FramelessWindow FDlg(this);
         FDlg.setWindowIcon(QIcon(":/acticons/coledit64.png"));
         FDlg.setWindowTitle("Add Multiple Columns");
         MulColDlg dlg(this);
         dlg.hnum = highest;
         dlg.pFwParent = &FDlg;

         FDlg.setContent(&dlg);

         FDlg.ContentDlg(&dlg);
         FDlg.show();

         // block
        dlg.setModal(true);
         if (dlg.exec() != QDialog::Accepted){
             SetControls(true);

             return;

         }


         if (dlg.num < 1)
             return;

         std::vector<size_t> rowpos;
         // add them
         Status("Adding columns...");
         ZTable& Tab = GetCurrentTable();


         GString fill = dlg.fill.toStdWString();
         for (int x = 1; x < dlg.num;++x)
         {
             QString name = "COUNTRY_" + QString::number(highest + x);
             ZColumn Col(name.toStdWString(),highest + x,EDataType::e_Float);
             size_t ni = Tab.Columns()[0].Items().size();
             Col.vItems.reserve(ni);
             for (size_t d = 0; d < ni; ++d){
                ZItem NIt(fill, d);
                Col.vItems.push_back(NIt);

             }



             Tab.Columns().push_back(Col);

         }
         if (dlg.addrows){
             // Add on first row
             for (int r = 1; r < dlg.num; ++r){
                 ZColumn& IDCol = Tab.Columns()[0];
                 ZItem Id( QString::number(highest + r).toStdWString(),IDCol.vItems.size());
                 IDCol.Items().push_back(Id);
             }

             // add on all columns
             for (int f = 1; f < dlg.num;++f){
                 auto It = Tab.Columns().begin() + 1; // skip the id column
                 while (It != Tab.Columns().end())
                 {

                     ZItem itm(fill, It->Items().size());
                     It->Items().push_back(itm);

                    ++It;

                 }

             }
         }
         SetControls(true);

         LoadTable(Tab);

         const int ftres = findTab(QString::fromStdWString(Tab.GetName()));
         if (ftres != -1){
             GenericTablePage* gtp = (GenericTablePage*)ui->tabWidget->widget(ftres);
             gtp->mDBTable = Tab;

         }


}


void MainWindow::on_actionAssociate_file_format_triggered()
{
    Associate();
}

template<typename Tp>
Tp ZTUtil::IStrStreamConv(const wstring &Arg)
{
   Tp ret;
    wistringstream wiss(Arg);
    wiss >> ret;
    return ret;


}
