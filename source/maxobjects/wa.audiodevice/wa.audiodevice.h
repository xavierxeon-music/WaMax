#ifndef AudioDeviceH
#define AudioDeviceH

#include "c74_min.h"
using namespace c74::min;

// see https://share.google/aimode/ZVNGBSsQE8mIaZZsA

class AudioDevice : public object<AudioDevice>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   AudioDevice(const atoms& args = {});
   ~AudioDevice();

private:
   t_object* adstatus; 
};

#endif // AudioDeviceH
