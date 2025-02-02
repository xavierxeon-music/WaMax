#ifndef MessageHandlerH
#define MessageHandlerH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <QtGlobal>

class MessageHandler : public object<MessageHandler>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   MessageHandler(const atoms& args = {});
   ~MessageHandler();

private:
   void maxMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
   static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
   static MessageHandler* me;
   QtMessageHandler defaultMessgeHandler;
};

#endif // MessageHandlerH
