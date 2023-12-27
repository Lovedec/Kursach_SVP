#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCustomPlot; // Объявление класса QCustomPlot

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QCustomPlot *customPlot;
};

#endif // MAINWINDOW_H
