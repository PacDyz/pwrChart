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
    textButton.setTextButton("Add time connections");
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("ListWithValuesModel", &listWithValues);
    context->setContextProperty("NameButton", &textButton);
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
/*    for(int h = from*60; h <= to*60; ++h)
    {
        numberConnectionOnHour.push_back({h,0});
    }*/
/*    for (uint32_t i = 0; i < valuesA.size(); ++i)
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
    }*/
/*    for(const auto& itr : numberConnectionOnHour)
    {
       series->append(itr.first, itr.second);
    }*/
    int numberElements{valuesA.size()};
    if(valuesB.size() < valuesA.size())
    {
        numberElements = valuesB.size();
    }
    std::sort(valuesA.begin(), valuesA.end());
    std::cout <<"sizeA = " << valuesA.size() << "sizeB = " << valuesB.size() << std::endl;
    for(int i = 0; i < numberElements; ++i )
    {
        std::cout << "value A " << valuesA.at(i) << "valuesB.at(i) = " << valuesB.at(i) << std::endl;
        if(valuesA.at(i) > this->to*60)
        {
            break;
        }
        series->append(valuesA.at(i), valuesB.at(i));
    }
    numberConnectionOnHour.clear();
    QChart* chart = new QChart();
    //QValueAxis *axisX = new QValueAxis;
    //axisX->setTickCount(10);
    //chart->addAxis(axisX, Qt::AlignBottom);
    //chart->legend()->hide();
    chart->addSeries(series);
    //series->attachAxis(axisX);
    chart->createDefaultAxes();
    chart->setTitle("Aplitude");
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    window.setCentralWidget(chartView);
    window.resize(1200, 800);
    window.show();
}

void MainWindow::setFilePath(QString filePath)
{
    const QUrl url(filePath);
    std::fstream myfile(QDir::toNativeSeparators(url.toLocalFile()).toUtf8().constData(), std::ios_base::in);
    std::cout << filePath.toUtf8().constData() << std::endl;
    float number{0};
    bool isFirstValue = true;
    A a;
    int counter = 0;
    int sumConnections = 0;
    std::string word;
    bool isFirstColumn = true;
        int counterRow = 0;
    if(!firstFile)
    {
        while(myfile >> word)
        {
            if(not isFirstColumn)
            {
                auto itr = word.find("E-05");
                if(itr != std::string::npos)
                {
                        word.erase(itr, 4);
                        valuesB.push_back(atoi( word.c_str() ) / std::pow(10,5) * averageConnectionTime);
                }
                valuesB.push_back(atoi( word.c_str() ) * averageConnectionTime);
                a.averageAmplitude = QString::number(atoi( word.c_str() ) * averageConnectionTime);
                isFirstColumn = true;
                qDebug() << "a = " << a.time << " " << a.averageAmplitude;
                listWithValues.append(a);
                a.time = "";
                a.averageAmplitude = "";
                counterRow++;
            }
            else
            {
                a.time = QString::fromStdString(word);
                valuesA.push_back(std::stoi( word));
                std::cout << word << std::endl;
                isFirstColumn = false;
                continue;
            }
        }
    }
    isFirstColumn = true;
//    int counterRow = 0;
    while (myfile >> number)
    {
        if(firstFile)
        {
            counter++;
            sumConnections += number;
        }
        else
        {
/*            if (isFirstValue)
            {
                a.time = QString::number(number);
                valuesA.push_back(number);
                isFirstValue = false;
                std::cout << number << std::endl;
                continue;
            }
            std::cout << number << std::endl;*/
//            a.averageAmplitude = QString::number(valuesB.at(counter));
//            valuesB.push_back(number);
/*            isFirstValue = true;
            qDebug() << "a = " << a.time << " " << a.averageAmplitude;
            listWithValues.append(a);
            a.time = "";
            a.averageAmplitude = "";
            counterRow++;*/
        }
    }
    if(!firstFile)
    {
/*        for (const auto& itr : valuesA)
        {
            std::cout << itr << std::endl;
        }
        for (const auto& itr : valuesB)
        {
            std::cout << itr << std::endl;
        }*/
        firstFile = true;
        textButton.setTextButton("Add time connections");
        emit textButton.textButtonChanged();
        return;
    }
    averageConnectionTime = sumConnections/counter;
    counter = 0;
    sumConnections = 0;
    textButton.setTextButton("Add number of connection per minute");
    emit textButton.textButtonChanged();
    firstFile = false;
}
