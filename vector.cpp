#include "vector.h"
#include <cstring>
Vector::Vector()
{
    v = new double[n];
    memset(v, 0, n * sizeof(double));
}

Vector::Vector(double x, double y, double z)
{
    v = new double[3];
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

Vector::Vector(const Vector& other)
{
    v = new double[n];
    memcpy(v, other.v, n*sizeof(double));
}

Vector::~Vector()
{
    delete v;
}
Vector& Vector::operator = (const Vector& other)
{
    memcpy(v, other.v, n*sizeof(double));
    return *this;
}

Vector& Vector::operator += (const Vector& other)
{
    for(int i = 1; i <= n; i++)
    {
        (*this)[i] += other[i];
    }
    return *this;
}

Vector& Vector::operator -= (const Vector& other)
{
    for(int i = 1; i <= n; i++)
    {
        (*this)[i] -= other[i];
    }
    return *this;
}

Vector& Vector::operator /= (const double& d)
{
    for(int i = 1; i <= n; i++)
    {
        (*this)[i] /= d;
    }
    return *this;
}

double& Vector::operator [](int i) const
{
    return v[i-1];
}

Vector Vector::operator + (const Vector& other) const
{
    Vector t;
    for(int i = 1; i <= n; i++)
    {
        t[i] = (*this)[i] + other[i];
    }
    return t;
}

Vector Vector::operator - (const Vector& other) const
{
    Vector t;
    for(int i = 1; i <= n; i++)
    {
        t[i] = (*this)[i] - other[i];
    }
    return t;
}

double Vector::operator * (const Vector& other) const
{
    double t = 0;
    for(int i = 1; i <= n; i++)
    {
        t += (*this)[i] * other[i];
    }
    return t;
}

Vector Vector::operator / (const double& d) const
{
    Vector t;
    for(int i = 1; i <= n; i++)
    {
        t[i] = (*this)[i] / d ;
    }
    return t;
}

Vector  Vector::operator - () const
{
    Vector t;
    for(int i = 1; i <= n; i++)
    {
        t[i] = -(*this)[i] ;
    }
    return t;
}

bool Vector::operator == (const Vector& other) const
{
    for(int i = 1; i <= n; i++)
    {
        if((*this)[i] != other[i])
            return false;
    }
    return true;
}

bool Vector::operator != (const Vector& other) const
{
    for(int i = 1; i <= n; i++)
    {
        if((*this)[i] != other[i])
            return true;
    }
    return false;
}

Vector Vector::ort(int i)
{
    Vector t;
    memset(t.v, 0, n * sizeof(double));
    t[i] = 1;
    return t;
}

ostream& operator << (ostream& os, const Vector& v)
{
    for(int i = 1; i <= Vector::n; i++){
        if(i > 1)
            os << "&";
        os << v[i];
    }
    return os;
}

Vector operator * (const double& d, const Vector& v)
{
    Vector t;
    for(int i = 1; i <= Vector::n; i++)
        t[i] = v[i] * d;
    return t;
}
