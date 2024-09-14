#include "wa.helpfile.h"

#include <string>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include <HelpForMax.h>
#include <patcher.h>

HelpFile::HelpFile(const atoms& args)
   : object<HelpFile>()
   , ui_operator::ui_operator(this, args)
   , paint{this, "paint", Patcher::minBind(this, &HelpFile::paintFunction)}
   , dblclick(this, "mousedoubleclick", Patcher::minBind(this, &HelpFile::mouseDoubleClickFunction))
   , setfile(this, "setfile", "set current patch file", Patcher::minBind(this, &HelpFile::setFileFunction))
   , loopTimer(this, Patcher::minBind(this, &HelpFile::timerFunction))
   , socket()
   , current("")
{
   loopTimer.delay(10);
}

atoms HelpFile::paintFunction(const atoms& args, const int inlet)
{
   target render(args);

   // background
   const color bg = color{0.0, 0.0, 0.0, 1.0};
   rect<fill>{render, bg};

   // field
   color highlight = color{0.7, 0.3, 0.3, 1.0};
   if (Current::State::NotInPackage == current.state)
      highlight = color{0.3, 0.3, 0.7, 1.0};
   else if (Current::State::HelpFileOutdated == current.state)
      highlight = color{0.7, 0.7, 0.3, 1.0};
   else if (Current::State::UpToDate == current.state)
      highlight = color{0.3, 0.7, 0.3, 1.0};

   rect<fill>{render, highlight, position{5.0, 5.0}, size{30.0, 30.0}};

   // the H
   if (Current::State::HelpFileOutdated == current.state || Current::State::UpToDate == current.state)
   {
      rect<fill>{render, bg, position{10.0, 10.0}, size{5.0, 20.0}};
      rect<fill>{render, bg, position{25.0, 10.0}, size{5.0, 20.0}};
   }
   if (QLocalSocket::ConnectedState == socket.state())
   {
      rect<fill>{render, bg, position{15.0, 17.5}, size{10.0, 5.0}};
   }

   return {};
}

atoms HelpFile::mouseDoubleClickFunction(const atoms& args, const int inlet)
{
   if (!HelpForMax::isServerActive())
   {
      cout << "start application" << endl;
      HelpForMax::startApplication();
   }

   return {};
}

atoms HelpFile::setFileFunction(const atoms& args, const int inlet)
{
   if (0 == args.size())
      return {};

   QString filePath = QString::fromStdString(args[0]);

#if defined(__APPLE__)
   const int slashIndex = filePath.indexOf("/", 1);
   const QString colonTest = (-1 != slashIndex) ? filePath.mid(slashIndex - 1, 1) : "";
   if (":" == colonTest)
   {
      const QString front = filePath.mid(0, slashIndex - 1);
      const QString back = filePath.mid(slashIndex);

      filePath = "/Volumes/" + front + back;
   }
#endif

   current = Current(filePath);
   current.checkState();
   //cout << "SET FILE " << current.patchPath.toStdString() << " " << (int)current.state << endl;
   sendData();
   redraw();

   return {};
}

atoms HelpFile::timerFunction(const atoms& args, const int inlet)
{
   if (QLocalSocket::UnconnectedState == socket.state())
      socket.connectToServer(HelpForMax::compileSockerName());

   if (Current::State::Initial != current.state || Current::State::NotInPackage != current.state)
   {
      auto readRead = [&]()
      {
         if (socket.state() != QLocalSocket::ConnectedState)
            return false;

         if (!socket.waitForReadyRead(10))
            return false;

         if (socket.bytesAvailable() == 0)
            return false;

         return true;
      };

      if (readRead())
         receiveData();
   }

   redraw();

   loopTimer.delay(500);
   return {};
}

void HelpFile::sendData()
{
   if (Current::State::NotInPackage == current.state)
      return;

   QJsonObject object;
   object["patch"] = current.patchPath;

   QJsonDocument doc(object);
   socket.write(doc.toJson(QJsonDocument::Compact));
   socket.flush();
}

void HelpFile::receiveData()
{
   const QJsonDocument doc = QJsonDocument::fromJson(socket.readAll());
   const QJsonObject object = doc.object();

   const QString path = object["patch"].toString();
   if (path != current.patchPath)
      return;

   current.checkState();
   redraw();
}

// main function

MIN_EXTERNAL(HelpFile);
