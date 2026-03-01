#ifndef McExtractH
#define McExtractH

#include "c74_min.h"
using namespace c74::min;

#include <MultichannelObject.h>

class McExtract : public MultichannelObject<McExtract>
{
public:
   MIN_DESCRIPTION{"extract single channel from a multichannel signal (1 based index)"};

public:
   McExtract(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output) override;
   int getChannelCount(long index) const override;
   void setChannelCount(long index, int count) override;

private:
   attribute<int> target;
   attribute<bool> strip;

   inlet<> input;
   outlet<> output;
   outlet<> single;
};

#endif // McExtractH
