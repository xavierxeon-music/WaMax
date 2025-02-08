#include "wa.noisegate_tilde.h"

NoiseGate::NoiseGate(const atoms& args)
   : object<NoiseGate>()
{
   cout << "noise gate" << endl;
}

MIN_EXTERNAL(NoiseGate);
