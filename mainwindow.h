#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>

#include "eventrecorder.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void form();
    void recorder();

private:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    EventRecorder erDialog;

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
