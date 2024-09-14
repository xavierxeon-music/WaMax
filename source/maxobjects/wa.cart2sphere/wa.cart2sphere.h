
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
   atoms xFunction(const atoms& args, const int inlet);
   atoms yFunction(const atoms& args, const int inlet);
   atoms zFunction(const atoms& args, const int inlet);
   atoms listFunction(const atoms& args, const int inlet);
   atoms calculateFunction(const atoms& args, const int inlet);
   void calcluate();

private:
   Linalg::Vector3 cartesian;
   outlet<> output;
   attribute<bool> asDegrees;
   message<> xMessage;
   message<> yMessage;
   message<> zMessage;
   message<> listMessage;
   message<> bangMessage;
   message<> doubleClick;
};

#endif // NOT  WaCart2SphereH
