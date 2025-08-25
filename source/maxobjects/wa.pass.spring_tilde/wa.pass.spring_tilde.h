#ifndef PassSpringH
#define PassSpringH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <AudioBlock.h>

class PassSpring : public object<PassSpring>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   PassSpring(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   using Outlet = std::unique_ptr<outlet<>>;

private:
   std::vector<Outlet> outletList;
   QList<AudioBlock*> audioBlocks;
   size_t timestamp;
};

#endif // PassSpringH
