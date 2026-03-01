#ifndef MultichannelObjectH
#define MultichannelObjectH

#include "c74_min.h"
using namespace c74::min;

template <typename McClass>
class MultichannelObject : public object<McClass>, public mc_operator<>
{
public:
   MultichannelObject();

public:
   virtual void operator()(audio_bundle input, audio_bundle output) = 0;
   virtual int getChannelCount(long index) const;
   virtual void setChannelCount(long index, int count);

protected:
   attribute<int> chans; // must have this name and type

private:
   static atoms maxClassSetupFunction(const atoms& args, const int inlet);

   static long compileMultChannelOutputCount(c74::max::t_object* x, long index, long count);
   static long inputChanged(c74::max::t_object* x, long index, long count);

private:
   message<> maxclassSetup;
};

#ifndef MultichannelObjectHPP
#include "MultichannelObject.hpp"
#endif // NOT MultichannelObjectHPP

#endif // NOT MultichannelObjectH
