#include "ChartWindow.hpp"
#include <QChart>
#include <iostream>

ChartWindow::ChartWindow(QWidget* parent) : engine{}
{
    QQmlContext* context = engine.rootContext();
    engine.load(QUrl(QStringLiteral("qrc:/chartWindow.qml")));
    if (engine.rootObjects().isEmpty())
    {
        std::cout << "error" << std::endl;
    }
    QLineSeries* series = new QLineSeries();
    series->add(0, 6);
    series->add(2, 4);
    this->chart()->addSeries(series);
    this->chart()->createDefaultAxes();
    // QObject* mainWindowQml = engine.rootObjects().first();
}
