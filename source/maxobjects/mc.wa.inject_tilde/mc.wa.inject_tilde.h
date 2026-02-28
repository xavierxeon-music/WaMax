#ifndef McInjectH
#define McInjectH

#include "c74_min.h"
using namespace c74::min;

class McInject : public object<McInject>, public mc_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   McInject(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   atoms maxClassSetupFunction(const atoms& args, const int inlet);

   static long compileMultChannelOutputCount(c74::max::t_object* x, long index, long count);
   static long inputChanged(c74::max::t_object* x, long index, long count);

private:
   attribute<int> chans; // must have this name and type
   message<> maxclassSetup;

   attribute<int> target;

   inlet<> input;
   inlet<> single;
   outlet<> output;
};

#endif // McInjectH
