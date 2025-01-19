#ifndef HostNameH
#define HostNameH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

class HostName : public object<HostName>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   HostName(const atoms& args = {});

private:
   atoms bangFunction(const atoms& args, const int inlet);

private:
   inlet<> input;

   outlet<> outputHostName;
   outlet<> outputIp;

   message<> bang;
};

#endif // HostNameH
