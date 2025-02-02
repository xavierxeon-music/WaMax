#include "wa_extension.qt_msgessage_handler.h"

#include <QString>

MessageHandler* MessageHandler::me = nullptr;

MessageHandler::MessageHandler(const atoms& args)
   : object<MessageHandler>()
   , defaultMessgeHandler()
{
   me = this;
   defaultMessgeHandler = qInstallMessageHandler(&MessageHandler::messageHandler);

   cout << "qt message handler created" << endl;
}

MessageHandler::~MessageHandler()
{
   qInstallMessageHandler(defaultMessgeHandler);
   me = nullptr;
}

void MessageHandler::maxMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
   switch (type)
   {
      case QtCriticalMsg:
      case QtFatalMsg:
         cerr << msg.toStdString() << endl;
         break;
      case QtWarningMsg:
         cwarn << msg.toStdString() << endl;
         break;
      default:
         cout << msg.toStdString() << endl;
         break;
   }

   defaultMessgeHandler(type, context, msg);
}

void MessageHandler::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
   if (!me)
      return;

   me->maxMessageHandler(type, context, msg);
}

MIN_EXTERNAL(MessageHandler);
