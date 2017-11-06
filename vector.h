#ifndef VECTOR_H
#define VECTOR_H

#include <fstream>

using namespace std;

class Vector {
    double * v;
    public:
        static const int n;
        double& operator [](int i) const;
        Vector();
        Vector(double x, double y, double z);
        Vector(const Vector& other);
        Vector& operator = (const Vector& other);
        Vector& operator += (const Vector& other);
        Vector& operator -= (const Vector& other);
        Vector& operator /= (const double& d);
        Vector operator + (const Vector& other) const;
        Vector operator - (const Vector& other) const;
        Vector operator / (const double& d) const;
        double operator * (const Vector& other) const;
        bool operator == (const Vector& other) const;
        bool operator != (const Vector& other) const;
        Vector operator - () const;
        static Vector ort(int i);
        ~Vector();
};

ostream& operator << (ostream& os, const Vector& v);

Vector operator * (const double& d, const Vector& v);

#endif
