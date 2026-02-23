#include "wa.maxscreen.h"

#include <filesystem>

#include <MaxPatcher.h>
#include <XXShared.h>

using ScreenServer = XX::Shared<"MaxScreen">;

MaxScreen::MaxScreen(const atoms& args)
   : object<MaxScreen>()
   , Max::QtJson()
   , socket()
   , inputMessage{this, "bang"}
   , inputDict{this, "dictionary", "dictionary"}
   , outputEvent{this, "event", "dictionary"}
   , outputState{this, "state", "dictionary"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreen::doubleClickFunction)}
   , bangMessage{this, "bang", "output state", Max::Patcher::minBind(this, &MaxScreen::bangFunction)}
   , openMessage{this, "open", Max::Patcher::minBind(this, &MaxScreen::openFunction)}
   , dictMessage{this, "dictionary", Max::Patcher::minBind(this, &MaxScreen::dictFunction)}
   , loadMessage{this, "load", "load file", Max::Patcher::minBind(this, &MaxScreen::loadFunction)}
   , unloadMessage{this, "unload", "unload file", Max::Patcher::minBind(this, &MaxScreen::unloadFunction)}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreen::loopTimerFunction))
   , loadTimer(this, Max::Patcher::minBind(this, &MaxScreen::loadTimerFunction))
   , eventDict{symbol(true)}
   , state()
   , stateDict{symbol(true)}
   , filename()
{
   loopTimer.delay(10);

   if (args.size() > 0)
   {
      filename = (const std::string)args[0];
      loadFile();
   }
}

atoms MaxScreen::doubleClickFunction(const atoms& args, const int inlet)
{
   startMaxScreen();
   loadFile();
   return {};
}

atoms MaxScreen::openFunction(const atoms& args, const int inlet)
{
   startMaxScreen();
   return {};
}

atoms MaxScreen::bangFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   copyToMaxDict(state, stateDict);
   outputState.send("dictionary", stateDict.name());

   return {};
}

atoms MaxScreen::dictFunction(const atoms& args, const int inlet)
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

atoms MaxScreen::loadFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   filename = (const std::string)args[0];
   loadFile();

   return {};
}

atoms MaxScreen::unloadFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   QJsonObject data;
   data["_type"] = "command";
   data["_id"] = "unload";

   QDataStream stream(&socket);
   stream << data;

   filename = std::string();

   return {};
}

atoms MaxScreen::loopTimerFunction(const atoms& args, const int inlet)
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

atoms MaxScreen::loadTimerFunction(const atoms& args, const int inlet)
{
   loadFile();
   return {};
}

void MaxScreen::startMaxScreen()
{
   if (ScreenServer::isServerActive())
      return;

   cout << "start max screen" << endl;

   ScreenServer::startApplication();
   loadFile();
}

void MaxScreen::loadFile()
{
   if (filename.empty())
      return;

   if (QLocalSocket::ConnectedState != socket.state())
   {
      loadTimer.delay(500);
      return;
   }

   QJsonObject data;
   data["_type"] = "command";
   data["_id"] = "load";

   std::filesystem::path filePath(filename);
   if (filePath.is_relative())
   {
      std::filesystem::path patchPath = Max::Patcher::path(this);
      patchPath = patchPath.remove_filename();

      data["path"] = QString::fromStdString(patchPath.string() + filename);
   }
   else
   {
      data["path"] = QString::fromStdString(filename);
   }

   QDataStream stream(&socket);
   stream << data;
}

void MaxScreen::receiveData()
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

MIN_EXTERNAL(MaxScreen);
