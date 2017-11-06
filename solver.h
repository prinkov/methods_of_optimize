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
#include <QThread>
#include "./parser/muParser.h"


class Solver : public QThread {
    Q_OBJECT
    int variant;
    bool html = false;
    bool latex = true;
public:
    Q_INVOKABLE void set(int v, int h, int l) {
        variant = v;
        html = h;
        latex = l;
    }

    Q_INVOKABLE void solve(int);
private:
    void makeIteration(const Vector& x);
    Vector Hooke_Jeeves(Vector x);
    Vector Coordinate_Descent(Vector x);
    Vector Rosenbrock(Vector x);
    Vector Powell (Vector x);
    Vector Nelder_Mead(Vector X);
signals:
    void sendMsg(QString msg);
    void progress(double val);
    void finish();
private:
    void run() {
        solve(variant);
        exec();
    }
};

#endif // SOLVER_H
