#include "wa.maxscreen.data.h"

#include <MaxPatcher.h>
#include <Shared.h>

using ScreenServer = Shared<"MaxScreen">;

MaxScreenData::MaxScreenData(const atoms& args)
   : object<MaxScreenData>()
   , Data()
   , socket()
   , input{this, "bang"}
   , outputSize{this, "screen size"}
   , outputTouchPoints{this, "touch points"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenData::openFunction)}
   , openMessage{this, "open", "open the maxscreen app", Max::Patcher::minBind(this, &MaxScreenData::openFunction)}
   , bangMessage{this, "bang", Max::Patcher::minBind(this, &MaxScreenData::bangFunction)}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreenData::timerFunction))
{
   loopTimer.delay(10);
}

MaxScreenData::~MaxScreenData()
{
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
   sendSize();
   sendTouchPoints();

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

   char marker;
   while (!stream.atEnd())
   {
      stream >> marker;

      if (Marker::ScreenSize == marker)
      {
         screenSize.load(stream);
         sendSize();
      }
      else if (Marker::TouchPoint == marker)
      {
         tpList.load(stream);
         sendTouchPoints();
      }
   }

   //socket.readAll();
}

void MaxScreenData::sendSize()
{
   atoms size = {screenSize.getWidth(), screenSize.getHeight()};
   outputSize.send(size);
}

void MaxScreenData::sendTouchPoints()
{
   for (int index = 0; index < tpList.size(); index++)
   {
      const TouchPoint::Entry& tp = tpList.at(index);
      atoms touchPoint = {index, tp.isPressed(), tp.getPosition().x(), tp.getPosition().y(), tp.getStart().x(), tp.getStart().y(), tp.getPressure(), tp.getArea()};
      outputTouchPoints.send(touchPoint);
   }
}

MIN_EXTERNAL(MaxScreenData);
