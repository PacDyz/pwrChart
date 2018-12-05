#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H
#include <QMainWindow>
#include <QQmlApplicationEngine>
#include "TelecomunicationChart.hpp"

class ChartWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChartWindow(QWidget* parent = 0);

public slots:

signals:

private:
    TelecomunicationChart* telecomunicationChart;
    QQmlApplicationEngine engine;
};

#endif // MAINWINDOW_H
