#ifndef GrowlH
#define GrowlH

#include "c74_min.h"
using namespace c74::min;

#include "GrowlVoice.h"

namespace Growl
{

   class Instance : public object<Instance>, public vector_operator<>
   {
   public:
      MIN_DESCRIPTION{"Death-metal growl synthesizer with vowels and consonants."};

   public:
      Instance(const atoms& args = {});

   public:
      void operator()(audio_bundle input, audio_bundle output);

   private:
      atoms numberFunction(const atoms& args, const int inlet);
      atoms listFunction(const atoms& args, const int inlet);
      atoms setupFunction(const atoms& args, const int inlet);
      Params snapshot() const;

   private:
      inlet<> in_gate;
      outlet<> out;

      attribute<number> freq;
      attribute<number> jitter;
      attribute<number> sub;
      attribute<number> noiseamt;
      attribute<number> fryrate;
      attribute<number> frydepth;
      attribute<number> drive;
      attribute<number> vowel;
      attribute<number> shift;
      attribute<number> width;
      attribute<number> burst;
      attribute<int> cstart;
      attribute<int> cend;
      attribute<number> att;
      attribute<number> rel;
      attribute<number> gain;

      message<> number;
      message<> list;
      message<> dspsetup;

      Voice voice;
      double gate{0.0};
      int note{-1};
   };

} // namespace Growl

#endif // GrowlH
