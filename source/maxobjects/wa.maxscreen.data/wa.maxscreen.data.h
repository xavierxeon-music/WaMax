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
   atoms doubleClickFunction(const atoms& args, const int inlet);
   atoms openFunction(const atoms& args, const int inlet);
   atoms bangFunction(const atoms& args, const int inlet);
   atoms dictFunction(const atoms& args, const int inlet);
   atoms loadFunction(const atoms& args, const int inlet);
   atoms unloadFunction(const atoms& args, const int inlet);

   atoms loopTimerFunction(const atoms& args, const int inlet);
   atoms loadTimerFunction(const atoms& args, const int inlet);

   void startMaxScreen();
   void loadFile();
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
   message<> dictMessage;
   message<> loadMessage;
   message<> unloadMessage;

   timer<timer_options::defer_delivery> loopTimer;
   timer<timer_options::defer_delivery> loadTimer;

   QJsonObject state;
   dict eventDict;
   dict stateDict;
   std::string filename;
};

#endif // MaxScreenDataH
