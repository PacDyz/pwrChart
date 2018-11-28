#include <QApplication>
#include "mainwindow.h"
#include "listwithvalues.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

   // qmlRegisterType<ListWithValues>("org.example", 1, 0, "ListWithValuesModel");

    MainWindow mainWindow;

    return app.exec();
}
