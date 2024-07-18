#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QTabWidget>
#include <QtSerialPort/QtSerialPort>

#include "lib/Protocol.h"

#include "scopewidget.h"
#include "counter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSerialPort* serialPort;

    QMenu *fileMenu;
    QAction *exitA, *connA, *nextA,*resetA;
    QTabWidget *tabW;

    Protocol* protocol;

    ScopeWidget *scopeW;
    counter *counterW;
    //general* gen;
    QLabel *comSB;
    QLabel *targetSB;
    QLabel *voltageSB;
    QLabel *confSB;
    QLabel *statusSB;

private slots:
    void quit();
    void connDialog();
    void readData();
    void nextCfg();
    void changeTargetName();
    void setDestMax(double vcc);


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
