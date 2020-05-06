#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "solver.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    qmlRegisterType<Solver>("xyz.prinkov", 1, 0, "Solver");

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
