#ifndef MaxScreenDataH
#define MaxScreenDataH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <QLocalSocket>

#include "Data.h"
#include "MaxQtJson.h"

class MaxScreenData : public object<MaxScreenData>,
                      public Max::QtJson,
                      public Data
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
   void updateState(const QJsonObject& data);

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
