#ifndef PipeDrainH
#define PipeDrainH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <AudioBlock.h>

class PipeDrain : public object<PipeDrain>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   PipeDrain(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   using Inlet = std::unique_ptr<inlet<>>;

private:
   std::vector<Inlet> inletList;
   AudioBlock::List audioBlocks;
};

#endif // PipeDrainH
