#ifndef GrowlH
#define GrowlH

#include "c74_min.h"
using namespace c74::min;

namespace Growl
{
   struct Noise
   {
      explicit Noise(uint32_t seed = 0x9E3779B9u);
      double operator()();

      uint32_t s;
   };

   struct Phasor
   {
      double operator()(double hz, double sr);

      double ph = 0.0;
      bool wrapped = false;
   };

   struct Slide
   {
      double operator()(double x, double up, double down);
      double y = 0.0;
   };

   struct RandomSah
   {
      double operator()(double hz, double sr, Noise& n);

      Phasor p;
      double held = 0.0;
   };

   struct SvfBP
   {
      double operator()(double x, double fc, double q, double sr);

      double ic1 = 0.0;
      double ic2 = 0.0;
   };

   struct DcBlock
   {
      double operator()(double x);

      double x1 = 0.0;
      double y1 = 0.0;
   };

   // ---------- consonants ----------

   struct ConsonantSpec
   {
      double fc;
      double q;
      double hold;
      double dec;
      double vot;
      double lvl;
      double asp;
      double ftr;
      bool roll;
   };

   struct ConsonantOut
   {
      double direct = 0, aspir = 0, vgate = 1, fmod = 1, rmod = 1;
   };

   class Consonant
   {
   public:
      void trigger(int idx);
      ConsonantOut process(double sr, Noise& noise);

   private:
      int cur_ = 0;
      double t_ = 1e9; // "long ago" so nothing fires at startup
      double rollPh_ = 0.0;
      SvfBP bp_;
   };

   // ---------- voice ----------

   struct Params
   {
      double freq = 85;
      double jitter = 0.03;
      double sub = 0.6;
      double noiseamt = 0.5;
      double fryrate = 35;
      double frydepth = 0.7;
      double drive = 6;
      double vowel = 1;
      double shift = 0.9;
      double width = 2.5;
      double burst = 0.8;
      double att = 8;
      double rel = 120;
      double gain = 2;
      int cstart = 0;
      double cend = 0;
   };

   class Voice
   {
   public:
      explicit Voice(uint32_t seed = 12345);

   public:
      void set_samplerate(double sr);
      double process(double gate, const Params& p);

   private:
      double ms2s(double ms) const;

   private:
      double sr_ = 44100.0;
      double prevGate_ = 0.0;
      Noise noise_;
      Phasor osc_;
      Phasor sub_;
      Phasor fry_;
      RandomSah jitSah_;
      RandomSah frySah_;
      Slide env_;
      Slide jitSlide_;
      Slide vowel_;
      SvfBP f1bp_;
      SvfBP f2bp_;
      SvfBP f3bp_;
      Consonant c1_;
      Consonant c2_;
      DcBlock dc_;
   };

   class Instance : public object<Instance>
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
