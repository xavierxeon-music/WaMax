#ifndef PipeTapH
#define PipeTapH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <AudioBlock.h>

class PipeTap : public object<PipeTap>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   PipeTap(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   using Outlet = std::unique_ptr<outlet<>>;

private:
   std::vector<Outlet> outletList;
   AudioBlock::List audioBlocks;
};

#endif // PipeTapH
