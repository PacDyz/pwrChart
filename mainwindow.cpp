#include "mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QQmlContext>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "ChartWindow.hpp"

MainWindow::MainWindow(QWidget* parent) : engine{}
{
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("ListWithValuesModel", &listWithValues);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) std::cout << "error" << std::endl;
    QObject* mainWindowQml = engine.rootObjects().first();
    QObject::connect(mainWindowQml, SIGNAL(getFilePath(QString)), this, SLOT(setFilePath(QString)));
    QObject::connect(mainWindowQml, SIGNAL(openChartWindow()), this, SLOT(openChart()));
}

void MainWindow::openChart()
{
    // ChartWindow chartWindow;
    QLineSeries* series = new QLineSeries();
    for (uint32_t i = 0; i < valuesA.size(); ++i)
    {
        series->append(valuesA.at(i), valuesB.at(i));
    }
    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->setCentralWidget(chartView);
    this->resize(400, 300);
    this->show();
}

void MainWindow::setFilePath(QString filePath)
{
    const QUrl url(filePath);
    std::fstream myfile(QDir::toNativeSeparators(url.toLocalFile()).toUtf8().constData(), std::ios_base::in);
    std::cout << filePath.toUtf8().constData() << std::endl;
    float number{0};
    bool isFirstValue = true;
    A a;
    while (myfile >> number)
    {
        if (isFirstValue)
        {
            a.time = QString::number(number);
            valuesA.push_back(number);
            isFirstValue = false;
            std::cout << number << std::endl;
            continue;
        }
        std::cout << number << std::endl;
        a.averageAmplitude = QString::number(number);
        valuesB.push_back(number);
        isFirstValue = true;
        qDebug() << "a = " << a.time << " " << a.averageAmplitude;
        listWithValues.append(a);
        a.time = "";
        a.averageAmplitude = "";
    }
    for (const auto& itr : valuesA)
    {
        std::cout << itr << std::endl;
    }
    for (const auto& itr : valuesB)
    {
        std::cout << itr << std::endl;
    }
    std::sort(valuesA.begin(), valuesA.end());
    std::cout << "HELLO" << std::endl;
}
