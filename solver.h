#ifndef SOLVER_H
#define SOLVER_H

#include <QScriptEngine>
#include <QDebug>
#include <QObject>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cstring>
#include "vector.h"
#include "./parser/muParser.h"


class Solver : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void run(int, bool, bool);
signals:
    void sendMsg(QString msg);
};

#endif // SOLVER_H
