#include "mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("MainWindow");
    setWindowTitle("RepDemo");
    resize(400, 300);

    centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    menuBar = new QMenuBar(this);
    menuBar->setObjectName("menuBar");

    menu_File = new QMenu;
    menu_File->setObjectName("menu_File");
    menu_File->setTitle(tr("&File"));
    menu_File->addAction(tr("F&orm"),this,SLOT(form()));
    menu_File->addAction(tr("&Recorder"),this,SLOT(recorder()));
    menuBar->addMenu(menu_File);
    setMenuBar(menuBar);

    recorder();
}

MainWindow::~MainWindow() {

}

void MainWindow::form() {
    Dialog().exec();
}

void MainWindow::recorder() {
    erDialog.show();
}

void MainWindow::closeEvent(QCloseEvent */*event*/) {
    erDialog.close();
}
