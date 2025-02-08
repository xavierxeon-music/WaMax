#ifndef NoiseGateH
#define NoiseGateH

#include "c74_min.h"
using namespace c74::min;

#include <SampleDelay.h>

// for mc wrapper see https://cycling74.com/forums/mc-support-with-min-api-for
// see https://github.com/acids-ircam/nn_tilde/blob/master/src/frontend/maxmsp/nn_tilde/nn_tilde.cpp

class NoiseGate : public object<NoiseGate>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   NoiseGate(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   using Inlet = std::unique_ptr<inlet<>>;
   using Outlet = std::unique_ptr<outlet<>>;

private:
   atoms dspSetupFunction(const atoms& args, const int inlet);

private:
   attribute<double> threshold;
   std::vector<Inlet> inletList;
   std::vector<Outlet> outletList;
   SampleDelay buffer;

   message<> dspsetup;

   int bufferSize;
   int portCount;
};

#endif // NoiseGateH
