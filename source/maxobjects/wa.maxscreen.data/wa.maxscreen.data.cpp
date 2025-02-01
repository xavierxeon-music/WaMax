#include "wa.maxscreen.data.h"

#include <MaxPatcher.h>
#include <Shared.h>

using ScreenServer = Shared<"MaxScreen">;

MaxScreenData::MaxScreenData(const atoms& args)
   : object<MaxScreenData>()
   , ScreenClient()
   , Data()
   , input{this, "bang"}
   , outputTouchPoints{this, "touch points"}
   , outputSize{this, "screen size"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenData::doubleClickFunction)}
   , bangMessage{this, "bang", Max::Patcher::minBind(this, &MaxScreenData::bangFunction)}
{
   startConnection();
}

atoms MaxScreenData::doubleClickFunction(const atoms& args, const int inlet)
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

void MaxScreenData::receiveData(QDataStream& stream)
{
   char marker;
   stream >> marker;

   if (Marker::ScreenSize == marker)
   {
      screenSize.load(stream);
      sendSize();
   }
   else if (Marker::TouchPoint == marker)
   {
      tpMap.load(stream);
      sendTouchPoints();
   }
}

void MaxScreenData::sendSize()
{
   atoms size = {screenSize.getWidth(), screenSize.getHeight()};
   outputSize.send(size);
}

void MaxScreenData::sendTouchPoints()
{
   for (TouchPoint::Map::const_iterator it = tpMap.constBegin(); it != tpMap.constEnd(); ++it)
   {
      const TouchPoint& tp = it.value();
      atoms touchPoint = {it.key(), tp.isPressed(), tp.getX(), tp.getY(), tp.getStartX(), tp.getStartY(), tp.getPressure(), tp.getArea()};
      outputTouchPoints.send(touchPoint);
   }
}

MIN_EXTERNAL(MaxScreenData);
