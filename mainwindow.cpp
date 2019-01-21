#include "mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QQmlContext>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "ChartWindow.hpp"
#include <QTextObject>

MainWindow::MainWindow(QWidget* parent) : engine{}
{
    textButton.setTextButton("Add durations of calls registered within one day");
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("ListWithValuesModel", &listWithValues);
    context->setContextProperty("NameButton", &textButton);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) std::cout << "error" << std::endl;
    QObject* mainWindowQml = engine.rootObjects().first();
    QObject::connect(mainWindowQml, SIGNAL(getFilePath(QString)), this, SLOT(setFilePath(QString)));
    QObject::connect(mainWindowQml, SIGNAL(openChartWindow()), this, SLOT(openChart()));
    QObject::connect(mainWindowQml, SIGNAL(saveConfiguration(int, int)), this, SLOT(saveRangeChart(int,int)));
    QObject::connect(mainWindowQml, SIGNAL(openHelpWindow()), this, SLOT(openHelp()));
}

void MainWindow::openHelp()
{
    helpWindow.resize(700, 500);
    QLabel *text = new QLabel{&helpWindow};
    text->resize(700,500);
    text->setText("How to use: \n"
              "First: \n"
              "choose file by button \"Add durations of calls registered within one day\" (after that button should change name)\n"
              "file from file system named czas\n"
              "Second:\n"
              "choose file by button \"Add with intensity of calls [minutes in day, number of calls]\" (button changed name)\n"
              "Third:\n"
              "click \"Generate Chart\"\n"
              "More:\n"
              "If you can changetime range, you can use edit field in top right and click SAVE CONFIGURATION\n");
    helpWindow.show();
}

void MainWindow::saveRangeChart(int from, int to)
{
    this->from = from;
    this->to = to;
}

void MainWindow::openChart()
{
    // ChartWindow chartWindow;
    QLineSeries* series = new QLineSeries();
    series->setName("Average Intensity");
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
    //std::sort(valuesA.begin(), valuesA.end());
    for(int i = 0; i < numberElements; ++i )
    {
        if(valuesA.at(i) >= this->to*60)
        {
            break;
        }
        if(valuesA.at(i) <= this->from*60)
        {
            continue;
        }
        series->append(valuesA.at(i)/60, valuesB.at(i));
    }
    numberConnectionOnHour.clear();
    QChart* chart = new QChart();
    //QValueAxis *axisX = new QValueAxis;
    //axisX->setTickCount(10);
    //chart->addAxis(axisX, Qt::AlignBottom);
    //chart->legend()->hide();
    chart->legend()->setVisible(true);
    chart->addSeries(series);
    //series->attachAxis(axisX);
    chart->createDefaultAxes();
    chart->setTitle("Average Intensity Chart");
    chart->axisY()->setTitleText("l.pol");
    chart->axisY()->setMin(0.0);
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("t[min]");
    axisX->setTickCount(to - from + 1);
    axisX->setLabelFormat("%.0f");
    axisX->setRange(this->from, this->to);
    chart->setAxisX(axisX, series);
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
    float number{0};
    bool isFirstValue = true;
    A a;
    int counter = 0;
    std::string word;
    bool isFirstColumn = true;
        int counterRow = 0;
    if(!firstFile)
    {
        while(myfile >> word)
        {
            if(not isFirstColumn)
            {
                auto itr = word.find(",");
                if(itr != std::string::npos)
                {
                    word.at(itr) = '.';
                }
                auto itr2 = word.find("E-0");
                if(itr2 != std::string::npos)
                {
                        int power = static_cast<int>(word.at(itr2 + 3)) - '0';
                        word.erase(itr2, 4);
                        valuesB.push_back(std::stod( word.c_str() ) / std::pow(10,power) * sumConnections);
                        a.averageAmplitude = QString::number(std::stod( word.c_str() ) / std::pow(10,power) * sumConnections);
                }
                else
                {
                    valuesB.push_back(std::stod( word.c_str() ) * sumConnections);
                    a.averageAmplitude = QString::number(std::stod( word.c_str() ) * sumConnections);
                }
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
        textButton.setTextButton("Add durations of calls registered within one day");
        emit textButton.textButtonChanged();
        sumConnections = 0;
        return;
    }
    averageConnectionTime = sumConnections/counter;
    counter = 0;
    textButton.setTextButton("Add with intensity of calls [minutes in day, number of calls]");
    emit textButton.textButtonChanged();
    firstFile = false;
}
