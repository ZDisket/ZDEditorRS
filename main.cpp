#include "mainwindow.h"
#include <QApplication>
#include "DarkStyle.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(new DarkStyle);
        FramelessWindow framelessWindow;

        MainWindow w;
        w.pParent = &framelessWindow;


        QIcon icon(":/ico/zediticopng.png");
        framelessWindow.setWindowIcon(icon);
        framelessWindow.setWindowTitle("ZDEditor RS+");
        framelessWindow.setContent(&w);
        framelessWindow.show();

        if (argc > 0)
            w.OpenFromArgs(argv[1]);

    return a.exec();
}
