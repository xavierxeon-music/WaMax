
#ifndef WaCart2SphereH
#define WaCart2SphereH

#include "c74_min.h"
using namespace c74::min;

#include <linalg.h>

class Cart2Sphere : public object<Cart2Sphere>
{
public:
   MIN_DESCRIPTION{"convert 3d carteasion coordinates to spherical"};

public:
   Cart2Sphere(const atoms& args = {});

private:
   Linalg::Vector3 cartesian;

public:
   inlet<> input;
   outlet<> output;

   message<> xValue;
   message<> yValue;
   message<> zValue;

   message<> doubleClick;

private:
   atoms xFunction(const atoms& args, const int inlet);
   atoms yFunction(const atoms& args, const int inlet);
   atoms zFunction(const atoms& args, const int inlet);

   atoms doubleClickedFunction(const atoms& args, const int inlet);

   void calcluate();
};

#endif // NOT  WaCart2SphereH
