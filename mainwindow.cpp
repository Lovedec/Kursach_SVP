#include "mainwindow.h"
#include <QFile>
#include <QVector>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    customPlot = new QCustomPlot(this);
    customPlot->setGeometry(50, 50, 700, 500);
    setCentralWidget(customPlot);

    // Выбор файла данных
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл данных", "", "Текстовые файлы (*.txt)");
    if (fileName.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Файл не выбран.");
        exit(0);
    }

    // Загрузка данных из файла
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл.");
        exit(0);
    }

    QVector<double> x, y;
    QTextStream in(&file);
    double value;
    int index = 0;
    while (!in.atEnd())
    {
        in >> value;
        x.append(index);
        y.append(value);
        index++;
    }

    // Вычисление периода сигнала
    int period = 0;
    double prevValue = y[0];
    for (int i = 1; i < x.size(); i++)
    {
        if ((prevValue >= 0 && y[i] < 0) || (prevValue < 0 && y[i] >= 0))
        {
            period = i;
            break;
        }
        prevValue = y[i];
    }

    // Установка размеров координатных осей
    customPlot->xAxis->setRange(0, 20);
    customPlot->yAxis->setRange(-10, 10);

    // Отображение графика сигнала
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->xAxis->setLabel("Время");
    customPlot->yAxis->setLabel("Значение");

    // Выделение периода сигнала
    if (period > 0) {
        customPlot->addGraph();
        QVector<double> periodX, periodY;
        for (int i = 0; i < period; i++)
        {
            periodX.append(x[i]);
            periodY.append(y[i] - y[0]);
        }
        customPlot->graph(1)->setData(periodX, periodY);
        customPlot->graph(1)->setPen(QPen(QColor(0, 255, 0)));
    }

    customPlot->replot();
}
