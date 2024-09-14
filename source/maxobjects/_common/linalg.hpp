#ifndef LinalgHPP
#define LinalgHPP

#include "linalg.h"

Linalg::Vector3::Vector3(const double& a, const double& b, const double& c)
   : a(a)
   , b(b)
   , c(c)
{
}

const double& Linalg::Vector3::getA() const
{
   return a;
}

void Linalg::Vector3::setA(const double& value)
{
   a = value;
}

const double& Linalg::Vector3::getB() const
{
   return b;
}

void Linalg::Vector3::setB(const double& value)
{
   b = value;
}

const double& Linalg::Vector3::getC() const
{
   return c;
}

void Linalg::Vector3::setC(const double& value)
{
   c = value;
}

#endif // NOT LinalgHPP