#ifndef MaxScreenDataH
#define MaxScreenDataH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <QLocalSocket>

#include "Data.h"

class MaxScreenData : public object<MaxScreenData>, public Data
{
public:
   MIN_DESCRIPTION{"max screen data"};

public:
   MaxScreenData(const atoms& args = {});
   ~MaxScreenData();

private:
   atoms timerFunction(const atoms& args, const int inlet);
   atoms openFunction(const atoms& args, const int inlet);
   atoms bangFunction(const atoms& args, const int inlet);

   void receiveData();
   void sendSize();
   void sendTouchPoints();
   void sendMouse();
   void sendPen();

private:
   QLocalSocket socket;

   inlet<> input;
   outlet<> outputSize;
   outlet<> outputTouchPointData;
   outlet<> outputTouchPointIndex;
   outlet<> outputMouse;
   outlet<> outputPen;

   message<> doubleClickMessage;
   message<> openMessage;
   message<> bangMessage;
   timer<timer_options::defer_delivery> loopTimer;
};

#endif // MaxScreenDataH
