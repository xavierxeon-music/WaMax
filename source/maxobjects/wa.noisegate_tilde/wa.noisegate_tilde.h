#ifndef NoiseGateH
#define NoiseGateH

#include "c74_min.h"
using namespace c74::min;

#include <SampleDelay.h>

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
   message<> dspsetup;

   SampleDelay buffer;
   int bufferSize;
   int portCount;
};

#endif // NoiseGateH
