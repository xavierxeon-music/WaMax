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
   , outputTouchPointData{this, "touch point data"}
   , outputTouchPointIndex{this, "touch point index"}
   , outputMouse{this, "mouse"}
   , outputPen{this, "pen"}
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
   sendMouse();
   sendPen();

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

      switch (marker)
      {
         case Marker::ScreenSize:
         {
            screenSize.load(stream);
            sendSize();
            break;
         }
         case Marker::TouchPoint:
         {
            tpList.load(stream);
            sendTouchPoints();
            break;
         }
         case Marker::Mouse:
         {
            mouse.load(stream);
            sendMouse();
            break;
         }
         case Marker::Pen:
         {
            pen.load(stream);
            sendPen();
            break;
         }
         default:
         {
            socket.readAll();
            break;
         }
      }
   }
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
      outputTouchPointIndex.send(index);

      const TouchPoint::Entry& tp = tpList.at(index);
      atoms touchPoint = {tp.isPressed(),
                          tp.getPosition().x(), tp.getPosition().y(),
                          tp.getStartPosition().x(), tp.getStartPosition().y(),
                          tp.getPressure(), tp.getArea()};
      outputTouchPointData.send(touchPoint);
   }
}

void MaxScreenData::sendMouse()
{
   atoms mouseData = {mouse.isPressed(),
                      mouse.getPosition().x(), mouse.getPosition().y(),
                      mouse.getStartPosition().x(), mouse.getStartPosition().y()};
   outputMouse.send(mouseData);
}

void MaxScreenData::sendPen()
{
}

MIN_EXTERNAL(MaxScreenData);
