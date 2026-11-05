#include "wa.audiodevice.h"

AudioDevice::AudioDevice(const atoms& args)
   : object<AudioDevice>()
   , adstatus(nullptr)
{
   t_atom ad_arg;
   atom_setsym(&ad_arg, gensym("driver"));

   // Step 2: Use Max API to instantiate the legacy class object
   // This instantiates a functional, headless adstatus engine
   adstatus = (t_object*)object_new_typed(CLASS_BOX, gensym("adstatus"), 1, &ad_arg);

   if (!adstatus)
   {
      cerr << "Failed to allocate the internal adstatus tracking module!" << endl;
   }
}

AudioDevice::~AudioDevice()
{
   if (adstatus)
      object_free(adstatus);
}

MIN_EXTERNAL(AudioDevice);
