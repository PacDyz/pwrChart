#include "mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QQmlContext>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "ChartWindow.hpp"
#include <QTextEdit>

MainWindow::MainWindow(QWidget* parent) : engine{}
{
    textButton.setTextButton("Dodaj czasy trwania połączeń zarejestrowanych w ciągu jednego dnia");
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
    helpWindow.resize(1100, 500);
    QTextEdit *text = new QTextEdit{&helpWindow};
    text->resize(1100,500);
    text->setText("Jak używać [podstawowa konfiguracja]: \n\n"
              "1. Wybrać plik przez naciśnięcie przycisku \"Dodaj czasy trwania połączeń zarejestrowanych w ciągu jednego dnia\" (po wybraniu następuje zmiana nazwy tego samego przycisku)\n"
              "przykładowy plik z danymi nazywa się \"czas\"\n\n"
              "2. Wybrać plik przez naciśnięcie przycisku \"Dodaj plik z intensywnością połączeń [minuty, liczba połączeń/liczba połączeń w całej dobie]\" (przycisk powraca do stanu z pierwszego punktu)\n\n"
              "3. Wygenerować wykres naciskając \"Generuj wykres\"\n\n"
              "Opcje dodatkowe:\n\n"
              "Zmiana zakresu czasowego: W prawym górnym rgu znajdują się dwa pola do edycji\n"
                  "gdzie możemy wpisać od jakiej do jakiej godziny chcemy wygenerować wykres, po uzupełnieniu pól należy kliknąć \"Zapisz konfiugrację\""
                  "po tym można wygenerować wykres jeśli wcześniej zostały wczytane dane z plików\n"
                  "\n"
                  "Lista (na podstawie tych wartości generowany jest wykres): "
                  "- kolumna nr. 1 - numery wierszy\n"
                  "- kolumna nr. 2 - minuta pomiaru w dobie\n"
                  "- kolumna nr. 3 liczba połączeń w danej minucie, po przeliczeniu ze stosunku [liczba połączeń w tej minucie/liczba połączeń w całej dobie]\n"
                  "\n"
                  "Formaty plików:\n\n"
                  "     1. Plik z czasami trwania połączeń zarejestrowanych w ciągu jednego dnia\n"
                  "         Liczby powinny:\n"
                  "             - być całkowite"
                  "             - liczby dodatnie\n"
                  "             - jednostka - minuty\n"
                  "             - nowa liczba w nowej linii\n"
                  "         Przykład:\n\n"
                  "         5\n"
                  "         10\n"
                  "         ...\n"
                  "         11\n\n"
                  "     2. Plik z intensywnością połączeń\n"
                  "         2.1 W pierwszej kolumnie powinny znajdować się minuty pomiaru w dobie\n"
                  "             Liczby powinny:\n"
                  "                 - być całkowite\n"
                  "                 - liczby dodatnie"
                  "                 - jednostka - minuty\n"
                  "                 - jedna liczba w wierszu i tej kolumnie\n"
                  "         2.2 W Drugiej kolumnie powinien znajdować się stosunek (liczby połączeń w tej minucie)/(liczby połączeń w całej dobie)\n"
                  "             Liczby powinny: \n"
                  "                 - mogą być z rozwinięciem dziesiętnym ( po przecinku )\n"
                  "                 - mogą być zapisane z exponentą ujemną (np. 1,23E-05)\n"
                  "                 - jedna liczba w wierszu i tej kolumnie\n"
                  "\n"
                  "         Przykład:\n"
                  "         10 0,002345\n"
                  "         28 1,25E-03\n"
                  "         ...\n"
                  "         14000 0,0111");
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
    //chart->createDefaultAxes();
    chart->setTitle("Średnie natężenie ruchu");
    //chart->axisY()->setTitleText("l.pol");
    //chart->axisY()->setMin(0.0);
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    axisX->setTitleText("t[h]");
    axisY->setTitleText("l.pol");
    axisX->setTickCount(to - from + 1);
    //axisY->setMin(0.0);
    axisY->setTickCount(10);
    chart->setAxisY(axisY, series);
    chart->axisY()->setMin(0.0);
    axisX->setLabelFormat("%.0f");
    axisY->setLabelFormat("%.0f");
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
                        valuesB.push_back(std::floor(std::stod( word.c_str() ) / std::pow(10,power) * sumConnections));
                        a.averageAmplitude = QString::number(std::floor(std::stod( word.c_str() ) / std::pow(10,power) * sumConnections));
                }
                else
                {
                    valuesB.push_back(std::floor(std::stod( word.c_str() ) * sumConnections));
                    a.averageAmplitude = QString::number(std::floor(std::stod( word.c_str() ) * sumConnections));
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
        textButton.setTextButton("Dodaj czas trwania połączeń zarejestrowanych w ciągu jednego dnia");
        emit textButton.textButtonChanged();
        sumConnections = 0;
        return;
    }
    averageConnectionTime = sumConnections/counter;
    counter = 0;
    textButton.setTextButton("Dodaj plik z intensywnością połączeń\n[minuty, liczba połączeń/liczba połączeń w całej dobie]");
    emit textButton.textButtonChanged();
    firstFile = false;
}
