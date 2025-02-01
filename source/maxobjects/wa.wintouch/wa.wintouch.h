#ifndef WinTouchH
#define WinTouchH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <QAbstractNativeEventFilter>

class WinTouch : public object<WinTouch>, public QAbstractNativeEventFilter
{
public:
   MIN_DESCRIPTION{"description"};

public:
   WinTouch(const atoms& args = {});

private:
   atoms rectFunction(const atoms& args, const int inlet);
   bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

private:
   inlet<> inputMessage;
   outlet<> outputTouch;

   message<> rect;
};

#endif // WinTouchH
