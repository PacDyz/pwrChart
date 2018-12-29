#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QQmlApplicationEngine>
#include "listwithvalues.h"
#include "namebutton.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);

public slots:
    void setFilePath(QString filePath);
    void openChart();
    void saveRangeChart(int from, int to);

private:
    std::vector<float> valuesA;
    std::vector<float> valuesB;
    QQmlApplicationEngine engine;
    ListWithValues listWithValues;
    int from{0};
    int to{24};
    int averageConnectionTime{0};
    NameButton textButton;
    bool firstFile{true};
};

#endif // MAINWINDOW_H
