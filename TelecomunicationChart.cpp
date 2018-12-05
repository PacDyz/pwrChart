#include "TelecomunicationChart.hpp"

TelecomunicationChart::TelecomunicationChart(QObject* parent) : QObject(parent)
{
    series = new QtCharts::QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
}
