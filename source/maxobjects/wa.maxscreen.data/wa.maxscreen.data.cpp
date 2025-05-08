#include "wa.maxscreen.data.h"

#include <MaxPatcher.h>
#include <Shared.h>

using ScreenServer = Shared<"MaxScreen">;

// see https://cycling74.com/forums/accessing-nested-dictionaries-in-min
// see https://gist.github.com/robtherich/19b12d27f5a31cd5e1e6af6de34fa65e

MaxScreenData::MaxScreenData(const atoms& args)
   : object<MaxScreenData>()
   , Max::QtJson()
   , socket()
   , inputMessage{this, "bang"}
   , inputDict{this, "dictionary", "dictionary"}
   , outputEvent{this, "event", "dictionary"}
   , outputState{this, "state", "dictionary"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenData::openFunction)}
   , openMessage{this, "open", "open the maxscreen app", Max::Patcher::minBind(this, &MaxScreenData::openFunction)}
   , bangMessage{this, "bang", Max::Patcher::minBind(this, &MaxScreenData::bangFunction)}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreenData::timerFunction))
   , eventDict{symbol(true)}
   , state()
   , stateDict{symbol(true)}
{
   loopTimer.delay(10);
}

atoms MaxScreenData::openFunction(const atoms& args, const int inlet)
{
   if (!ScreenServer::isServerActive())
   {
      cout << "start max screen" << endl;

      ScreenServer::startApplication();
   }
   return {};
}

atoms MaxScreenData::bangFunction(const atoms& args, const int inlet)
{
   copyToMaxDict(state, stateDict);
   outputState.send("dictionary", stateDict.name());

   return {};
}

atoms MaxScreenData::timerFunction(const atoms& args, const int inlet)
{
   if (QLocalSocket::UnconnectedState == socket.state())
   {
      socket.connectToServer(ScreenServer::socketName());
      loopTimer.delay(500);
   }
   else if (QLocalSocket::ConnectedState == socket.state())
   {
      auto readyRead = [&]()
      {
         if (!socket.waitForReadyRead(10))
            return false;

         if (socket.bytesAvailable() == 0)
            return false;

         return true;
      };

      if (readyRead())
         receiveData();

      loopTimer.delay(100);
   }
   else
   {
      loopTimer.delay(500);
   }

   return {};
}

void MaxScreenData::receiveData()
{
   QDataStream stream(&socket);

   QJsonObject data;

   while (!stream.atEnd())
   {
      stream >> data;

      copyToMaxDict(data, eventDict);
      outputEvent.send("dictionary", eventDict.name());

      mergeDicts(state, data);
   }
}

MIN_EXTERNAL(MaxScreenData);
