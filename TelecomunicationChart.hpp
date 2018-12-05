#ifndef TELECOMUNICATIONCHART_HPP
#define TELECOMUNICATIONCHART_HPP
#include <QObject>
#include <QtCharts>

class TelecomunicationChart : public QObject
{
    Q_OBJECT
public:
    explicit TelecomunicationChart(QObject* parent = 0);
    ~TelecomunicationChart(){};
    QtCharts::QLineSeries* getSeries() const { return series; };

private:
    QtCharts::QLineSeries* series;
};

#endif // TELECOMUNICATIONCHART_HPP
