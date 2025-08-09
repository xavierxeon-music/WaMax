#ifndef MaxScreenDataH
#define MaxScreenDataH

// keep order
#include "c74_min.h"
using namespace c74::min;
#include <CommonQt.h>
#include <MaxQtJson.h>

#include <QLocalSocket>

class MaxScreenData : public object<MaxScreenData>,
                      public Max::QtJson
{
public:
   MIN_DESCRIPTION{"max screen data"};

public:
   MaxScreenData(const atoms& args = {});

private:
   atoms timerFunction(const atoms& args, const int inlet);
   atoms openFunction(const atoms& args, const int inlet);
   atoms bangFunction(const atoms& args, const int inlet);

   void receiveData();

private:
   QLocalSocket socket;

   inlet<> inputMessage;
   inlet<> inputDict;
   outlet<> outputEvent;
   outlet<> outputState;

   message<> doubleClickMessage;
   message<> openMessage;
   message<> bangMessage;
   timer<timer_options::defer_delivery> loopTimer;

   QJsonObject state;
   dict eventDict;
   dict stateDict;
};

#endif // MaxScreenDataH
