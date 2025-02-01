#ifndef MaxScreenDataH
#define MaxScreenDataH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include "ScreenClient.h" // base class

#include "Data.h"

class MaxScreenData : public object<MaxScreenData>, public ScreenClient, public Data
{
public:
   MIN_DESCRIPTION{"max screen data"};

public:
   MaxScreenData(const atoms& args = {});

private:
   atoms doubleClickFunction(const atoms& args, const int inlet);
   atoms bangFunction(const atoms& args, const int inlet);

   void receiveData(QDataStream& stream) override;
   void sendSize();
   void sendTouchPoints();

private:
   inlet<> input;
   outlet<> outputTouchPoints;
   outlet<> outputSize;

   message<> doubleClickMessage;
   message<> bangMessage;
};

#endif // MaxScreenDataH
