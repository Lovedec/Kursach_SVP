#include "mainwindow.h"
#include <QFile>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    customPlot = new QCustomPlot(this);
    customPlot->setMinimumSize(800, 600);
    customPlot->setWindowTitle("Дискретный сигнал");

    // Загрузка данных из файла
    QFile file("sign4.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл.");
        exit(0);
    }
    QTextStream in(&file);
    QVector<double> x, y;
    int i = 0;
    while (!in.atEnd())
    {
        double val;
        in >> val;
        x.append(i);
        y.append(val);
        i++;
    }
    file.close();

    // Вычисление периода сигнала
    int period = 0;
    int peakIndex = 0;
    double peakValue = y[0];
    for (int j = 1; j < y.size(); j++)
    {
        if (y[j] > peakValue)
        {
            peakValue = y[j];
            peakIndex = j;
        }
    }

    for (int j = peakIndex + 1; j < y.size(); j++)
    {
        if (y[j] < peakValue)
        {
            period = j - peakIndex;
            break;
        }
    }

    // Отображение графика
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->xAxis->setLabel("Время");
    customPlot->yAxis->setLabel("Значение");

    // Выделение периодического участка
    if (period != 0) {
        QVector<double> periodX, periodY;
        for (int j = 0; j < x.size(); j += period)
        {
            periodX.append(x[j]);
            periodY.append(y[j]);
        }
        customPlot->addGraph();
        customPlot->graph(1)->setData(periodX, periodY);
        customPlot->graph(1)->setPen(QPen(Qt::green));
    }

    customPlot->replot();
    setCentralWidget(customPlot);
}

MainWindow::~MainWindow()
{
    delete customPlot;
}
