#include "wa.maxscreen.data.h"

#include <MaxPatcher.h>
#include <Shared.h>

using ScreenServer = Shared<"MaxScreen">;

// see https://cycling74.com/forums/accessing-nested-dictionaries-in-min
// see https://gist.github.com/robtherich/19b12d27f5a31cd5e1e6af6de34fa65e

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
   , outputDict{this, "dict", "dictionary"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenData::openFunction)}
   , openMessage{this, "open", "open the maxscreen app", Max::Patcher::minBind(this, &MaxScreenData::openFunction)}
   , bangMessage{this, "bang", Max::Patcher::minBind(this, &MaxScreenData::bangFunction)}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreenData::timerFunction))
   , state()
   , stateDict{symbol(true)}
{
   loopTimer.delay(10);

   {
      using namespace c74::max;
      common_symbols_init();
   }
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

   convertStateToDict();
   outputDict.send("dictionary", stateDict.name());

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
      updateState(data);

      if (tpList.load(data))
         sendTouchPoints();
      else if (screenSize.load(data))
         sendSize();
      else if (mouse.load(data))
         sendMouse();
      else if (pen.load(data))
         sendPen();
   }

   convertStateToDict();
   outputDict.send("dictionary", stateDict.name());
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

void MaxScreenData::updateState(const QJsonObject& data)
{
   for (const QString& key : data.keys())
   {
      state[key] = data[key];
   }
}

void MaxScreenData::convertStateToDict()
{
   const QJsonDocument doc(state);
   const QByteArray jsonData = doc.toJson();

   //cout << jsonData.constData() << endl;

   using namespace c74::max;
   t_atom result[1];

   t_object* jsonreader = (t_object*)object_new(_sym_nobox, _sym_jsonreader); // can not reuse this object
   t_object* dictObject = nullptr;

   auto parseAndCopy = [&]()
   {
      t_max_err parserError = (t_max_err)object_method(jsonreader, _sym_parse, jsonData.constData(), result);
      if (parserError)
      {
         cerr << "error parsing json data" << endl;
         return;
      }

      dictObject = (t_object*)atom_getobj(result);
      if (!dictObject)
      {
         cerr << "error getting dictionary from json data" << endl;
         return;
      }

      if (object_classname_compare(dictObject, _sym_dictionary))
      {
         stateDict.clear();
         stateDict = dict{(t_dictionary*)dictObject};
         stateDict.touch();
      }
      else
      {
         cerr << "error: object is not a dictionary" << endl;
      }
   };

   parseAndCopy();

   object_free(jsonreader);

   if (dictObject)
      object_free(dictObject);
}

MIN_EXTERNAL(MaxScreenData);
