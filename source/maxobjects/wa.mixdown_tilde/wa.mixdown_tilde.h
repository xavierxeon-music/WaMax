#ifndef MixDownH
#define MixDownH

#include "c74_min.h"
using namespace c74::min;

class MixDown : public object<MixDown>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"add n signal channels together"};

public:
   MixDown(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   using Inlet = std::unique_ptr<inlet<>>;

private:
   attribute<bool> normalize;

   std::vector<Inlet> inletList;
   outlet<> output;
};

#endif // MixDownH
