#ifndef QtApplicationH
#define QtApplicationH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

class QtApplication : public object<QtApplication>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   QtApplication(const atoms& args = {});
   ~QtApplication();

private:
   atoms timerFunction(const atoms& args, const int inlet);

private:
   timer<timer_options::defer_delivery> eventLoopTimer;
   //timer<> eventLoopTimer;
};

#endif // QtApplicationH
