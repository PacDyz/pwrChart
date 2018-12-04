#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QQmlApplicationEngine>
#include "listwithvalues.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);

public slots:
    void setFilePath(QString filePath);
    void openChart();

private:
    std::vector<float> valuesA;
    std::vector<float> valuesB;
    QQmlApplicationEngine engine;
    ListWithValues listWithValues;
};

#endif // MAINWINDOW_H
