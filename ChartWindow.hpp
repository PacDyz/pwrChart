#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H
#include <QMainWindow>
#include <QQmlApplicationEngine>

class ChartWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChartWindow(QWidget* parent = 0);

public slots:

private:
    QQmlApplicationEngine engine;
};

#endif // MAINWINDOW_H
