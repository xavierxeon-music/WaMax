#ifndef PassDrainH
#define PassDrainH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <AudioBlock.h>

class PassDrain : public object<PassDrain>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   PassDrain(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   using Inlet = std::unique_ptr<inlet<>>;

private:
   std::vector<Inlet> inletList;
   QList<AudioBlock*> audioBlocks;
};

#endif // PassDrainH
