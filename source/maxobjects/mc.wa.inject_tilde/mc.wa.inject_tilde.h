#ifndef McInjectH
#define McInjectH

#include "c74_min.h"
using namespace c74::min;

#include <MultichannelObject.h>

class McInject : public MultichannelObject<McInject>
{
public:
   MIN_DESCRIPTION{"inject single channel into a multichannel signal (1 based index)"};

public:
   McInject(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output) override;
   int getChannelCount(long index) const override;
   void setChannelCount(long index, int count) override;

private:
   attribute<int> target;

   inlet<> input;
   inlet<> single;
   outlet<> output;
};

#endif // McInjectH
