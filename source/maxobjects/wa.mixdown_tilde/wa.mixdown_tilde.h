#ifndef MixDownH
#define MixDownH

#include "c74_min.h"
using namespace c74::min;

class MixDown : public object<MixDown>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   MixDown(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   using Inlet = std::unique_ptr<inlet<>>;
   using Outlet = std::unique_ptr<outlet<>>;

private:
   attribute<bool> normalize;

   std::vector<Inlet> inletList;
   std::vector<Outlet> outletList;
};

#endif // MixDownH
