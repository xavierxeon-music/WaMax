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
   , bangMessage{this, "bang", "output state", Max::Patcher::minBind(this, &MaxScreenData::bangFunction)}
   , dictMessage{this, "dictionary", Max::Patcher::minBind(this, &MaxScreenData::dictFunction)}
   , loadMessage{this, "load", "load file", Max::Patcher::minBind(this, &MaxScreenData::loadFunction)}
   , unloadMessage{this, "unload", "unload file", Max::Patcher::minBind(this, &MaxScreenData::unloadFunction)}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreenData::timerFunction))
   , eventDict{symbol(true)}
   , state()
   , stateDict{symbol(true)}
{
   loopTimer.delay(10);
}

atoms MaxScreenData::openFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   if (!ScreenServer::isServerActive())
   {
      cout << "start max screen" << endl;

      ScreenServer::startApplication();
   }
   return {};
}

atoms MaxScreenData::bangFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   copyToMaxDict(state, stateDict);
   outputState.send("dictionary", stateDict.name());

   return {};
}

atoms MaxScreenData::dictFunction(const atoms& args, const int inlet)
{
   if (1 != inlet)
      return {};

   const dict inputDict{args[0]};
   QJsonObject data = fromMaxDict(inputDict);

   if (data.isEmpty())
      return {};

   QDataStream stream(&socket);
   stream << data;

   return {};
}

atoms MaxScreenData::loadFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   const std::string filename = args[0];

   QJsonObject data;
   data["_type"] = "command";
   data["_id"] = "load";
   data["path"] = QString::fromStdString(filename);

   QDataStream stream(&socket);
   stream << data;

   return {};
}

atoms MaxScreenData::unloadFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   QJsonObject data;
   data["_type"] = "command";
   data["_id"] = "unload";

   QDataStream stream(&socket);
   stream << data;

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

      const QString type = data["_type"].toString();
      const QString id = data["_id"].toString();
      if (!type.isEmpty() && !id.isEmpty())
      {
         QJsonObject typeObject = state[type].toObject();
         typeObject[id] = data;
         state[type] = typeObject;
      }
   }
}

MIN_EXTERNAL(MaxScreenData);
