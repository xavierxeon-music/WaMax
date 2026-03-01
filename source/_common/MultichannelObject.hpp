#ifndef MultichannelObjectHPP
#define MultichannelObjectHPP

#include "MultichannelObject.h"

template <typename McClass>
MultichannelObject<McClass>::MultichannelObject()
   : object<McClass>()
   , mc_operator<>()
   , chans{this, "chans", 1, range{1, 1024}}
   , maxclassSetup{this, "maxclass_setup", &MultichannelObject<McClass>::maxClassSetupFunction}
{
}

template <typename McClass>
atoms MultichannelObject<McClass>::maxClassSetupFunction(const atoms& args, const int inlet)
{
   c74::max::t_class* c = args[0];
   c74::max::class_addmethod(c, (c74::max::method)MultichannelObject<McClass>::compileMultChannelOutputCount, "multichanneloutputs", c74::max::A_CANT, 0);
   c74::max::class_addmethod(c, (c74::max::method)MultichannelObject<McClass>::inputChanged, "inputchanged", c74::max::A_CANT, 0);

   return {};
}

template <typename McClass>
long MultichannelObject<McClass>::compileMultChannelOutputCount(c74::max::t_object* x, long index, long count)
{
   minwrap<McClass>* ob = (minwrap<McClass>*)(x);

   const int channelCount = ob->m_min_object.chans;
   return channelCount;
}

template <typename McClass>
long MultichannelObject<McClass>::inputChanged(c74::max::t_object* x, long index, long count)
{
   minwrap<McClass>* ob = (minwrap<McClass>*)(x);
   if (0 == index)
      ob->m_min_object.chans = count;

   return true;
}

#endif // NOT MultichannelObjectHPP
