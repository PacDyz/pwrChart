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
    QObject::connect(mainWindowQml, SIGNAL(saveConfiguration(int, int)), this, SLOT(saveRangeChart(int,int)));
}

void MainWindow::saveRangeChart(int from, int to)
{
    std::cout << "range " << from << " " << to << std::endl;
    this->from = from;
    this->to = to;
    std::cout << "range " << this->from << " " << this->to << std::endl;
}

void MainWindow::openChart()
{
    // ChartWindow chartWindow;
    std::cout << "from " << from << std::endl;
    std::cout << "to " << to  << std::endl;
    QLineSeries* series = new QLineSeries();
    std::vector<std::pair<int, int>> numberConnectionOnHour;
    for(int h = from; h <= to; ++h)
    {
        numberConnectionOnHour.push_back({h,0});
    }
    for (uint32_t i = 0; i < valuesA.size(); ++i)
    {
        auto itr = std::find_if(std::begin(numberConnectionOnHour), std::end(numberConnectionOnHour),
                     [this, &i](auto par)
        {return (par.first >= static_cast<int>(valuesA.at(i) +1)) &&
                    (par.first <= static_cast<int>(valuesB.at(i) + valuesA.at(i) + 1));});
        if(itr != std::end(numberConnectionOnHour))
        {
            itr->second++;
        }
        //series->append(valuesA.at(i) + 1, valuesB.at(i));
    }
    for(const auto& itr : numberConnectionOnHour)
    {
       series->append(itr.first, itr.second);
    }
    numberConnectionOnHour.clear();
    QChart* chart = new QChart();
    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(10);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->legend()->hide();
    chart->addSeries(series);
    series->attachAxis(axisX);
    chart->createDefaultAxes();
    chart->setTitle("Aplitude");
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->setCentralWidget(chartView);
    this->resize(700, 500);
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
