#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace growl
{

   constexpr double kPi = 3.14159265358979323846;

   // ---------- building blocks (each mirrors a gen~ operator) ----------

   struct Noise
   { // xorshift32, -1..1
      uint32_t s;
      explicit Noise(uint32_t seed = 0x9E3779B9u)
         : s(seed ? seed : 1)
      {
      }
      double operator()()
      {
         s ^= s << 13;
         s ^= s >> 17;
         s ^= s << 5;
         return s * (2.0 / 4294967296.0) - 1.0;
      }
   };

   struct Phasor
   { // gen phasor()
      double ph = 0.0;
      bool wrapped = false;
      double operator()(double hz, double sr)
      {
         ph += hz / sr;
         wrapped = ph >= 1.0;
         if (wrapped)
            ph -= std::floor(ph);
         return ph;
      }
   };

   struct Slide
   { // gen slide()
      double y = 0.0;
      double operator()(double x, double up, double down)
      {
         const double n = std::max(x > y ? up : down, 1.0);
         y += (x - y) / n;
         return y;
      }
   };

   struct RandomSah
   { // sah(noise(), phasor(hz), 0.5)
      Phasor p;
      double held = 0.0;
      double operator()(double hz, double sr, Noise& n)
      {
         p(hz, sr);
         if (p.wrapped)
            held = n();
         return held;
      }
   };

   struct SvfBP
   { // TPT bandpass, unity peak
      double ic1 = 0.0, ic2 = 0.0;
      double operator()(double x, double fc, double q, double sr)
      {
         fc = std::clamp(fc, 20.0, sr * 0.45);
         const double g = std::tan(kPi * fc / sr);
         const double k = 1.0 / q;
         const double a1 = 1.0 / (1.0 + g * (g + k));
         const double a2 = g * a1, a3 = g * a2;
         const double v3 = x - ic2;
         const double v1 = a1 * ic1 + a2 * v3;
         const double v2 = ic2 + a2 * ic1 + a3 * v3;
         ic1 = 2.0 * v1 - ic1;
         ic2 = 2.0 * v2 - ic2;
         return v1 * k;
      }
   };

   struct DcBlock
   { // gen dcblock
      double x1 = 0.0, y1 = 0.0;
      double operator()(double x)
      {
         const double y = x - x1 + 0.9997 * y1;
         x1 = x;
         y1 = y;
         return y;
      }
   };

   inline double vowel3(double v, double u, double o, double a)
   {
      return v < 1.0 ? u + (o - u) * v : o + (a - o) * (v - 1.0);
   }

   // ---------- consonants ----------

   struct ConsonantSpec
   {
      double fc, q, hold, dec, vot, lvl, asp, ftr;
      bool roll;
   };

   inline constexpr ConsonantSpec kConsonants[] = {
      //  fc     q    hold  dec  vot   lvl  asp   ftr    roll
      {1000, 1.0, 0, 1, 0, 0.0, 0.0, 0.00, false},      // 0  none
      {1800, 2.5, 2, 20, 45, 1.0, 0.6, 0.15, false},    // 1  K
      {1500, 2.5, 1, 12, 10, 0.7, 0.1, 0.15, false},    // 2  G
      {4200, 1.8, 2, 18, 40, 0.9, 0.5, 0.10, false},    // 3  T
      {3500, 1.8, 1, 10, 8, 0.6, 0.1, 0.10, false},     // 4  D
      {900, 1.2, 1, 12, 40, 0.8, 0.6, -0.30, false},    // 5  P
      {700, 1.2, 1, 8, 5, 0.5, 0.1, -0.30, false},      // 6  B
      {6500, 3.0, 110, 40, 120, 0.6, 0.0, 0.00, false}, // 7  S
      {2800, 2.0, 110, 40, 120, 0.7, 0.0, 0.00, false}, // 8  SH
      {1500, 0.7, 60, 30, 60, 0.2, 1.0, 0.00, false},   // 9  H
      {1000, 1.0, 120, 30, 0, 0.0, 0.0, 0.00, true},    // 10 RR
   };
   constexpr int kNumConsonants = static_cast<int>(std::size(kConsonants));

   struct ConsonantOut
   {
      double direct = 0, aspir = 0, vgate = 1, fmod = 1, rmod = 1;
   };

   class Consonant
   {
   public:
      void trigger(int idx)
      {
         cur_ = std::clamp(idx, 0, kNumConsonants - 1);
         t_ = 0.0;
         rollPh_ = 0.0;
      }

      ConsonantOut process(double sr, Noise& noise)
      {
         ConsonantOut o;
         const auto& c = kConsonants[cur_];
         const double ms = t_ * 1000.0 / sr;
         t_ += 1.0;
         if (cur_ == 0)
            return o;

         const double env = ms < c.hold ? 1.0 : std::exp(-(ms - c.hold) / c.dec);
         const double nz = noise();
         o.direct = bp_(nz, c.fc, c.q, sr) * env * c.lvl;
         o.aspir = nz * env * c.asp;
         o.vgate = ms >= c.vot ? 1.0 : 0.0;
         o.fmod = 1.0 + c.ftr * std::exp(-ms / 40.0);
         if (c.roll && ms < c.hold)
         {
            rollPh_ += 28.0 / sr;
            o.rmod = 0.4 + 0.6 * std::cos(2.0 * kPi * rollPh_);
         }
         return o;
      }

   private:
      int cur_ = 0;
      double t_ = 1e9; // "long ago" so nothing fires at startup
      double rollPh_ = 0.0;
      SvfBP bp_;
   };

   // ---------- voice ----------

   struct Params
   {
      double freq = 85, jitter = 0.03, sub = 0.6, noiseamt = 0.5;
      double fryrate = 35, frydepth = 0.7, drive = 6;
      double vowel = 1, shift = 0.9, width = 2.5, burst = 0.8;
      double att = 8, rel = 120, gain = 2;
      int cstart = 0, cend = 0;
   };

   class Voice
   {
   public:
      explicit Voice(uint32_t seed = 12345)
         : noise_(seed)
      {
      }
      void set_samplerate(double sr) { sr_ = sr; }

      double process(double gate, const Params& p)
      {
         // triggers
         const bool on = gate > 0.5 && prevGate_ <= 0.5;
         const bool off = gate <= 0.5 && prevGate_ > 0.5;
         prevGate_ = gate;
         if (on)
            c1_.trigger(p.cstart);
         if (off)
            c2_.trigger(p.cend);
         const auto k1 = c1_.process(sr_, noise_);
         const auto k2 = c2_.process(sr_, noise_);

         const double env = env_(gate * k1.vgate, ms2s(p.att), ms2s(p.rel));

         // source
         const double jit = jitSlide_(jitSah_(23.0, sr_, noise_), 200, 200);
         const double f = p.freq * (1.0 + p.jitter * jit);
         const double src = (osc_(f, sr_) * 2 - 1) + p.sub * (sub_(f * 0.5, sr_) * 2 - 1) + p.noiseamt * noise_();

         // fry
         const double fr = p.fryrate * (1.0 + 0.3 * frySah_(p.fryrate, sr_, noise_));
         double q = 1.0 - fry_(fr, sr_);
         q *= q;
         q *= q; // pow(.,4)
         const double pulse = q * 3.0;
         double x = src * (1.0 + (pulse - 1.0) * p.frydepth) * k1.rmod;

         // distortion, envelope, aspiration
         x = std::tanh(x * p.drive) * env + (k1.aspir + k2.aspir) * p.burst;

         // formants
         const double vs = vowel_(std::clamp(p.vowel, 0.0, 2.0), 400, 400);
         const double sh = p.shift * k1.fmod * k2.fmod;
         const double f1 = vowel3(vs, 350, 400, 600) * sh;
         const double f2 = vowel3(vs, 600, 750, 1040) * sh;
         const double f3 = vowel3(vs, 2400, 2400, 2250) * sh;
         const double g2 = vowel3(vs, 0.10, 0.28, 0.45);
         const double g3 = vowel3(vs, 0.025, 0.09, 0.35);
         const double b1 = vowel3(vs, 40, 40, 60) * p.width;
         const double b2 = vowel3(vs, 80, 80, 70) * p.width;
         const double b3 = vowel3(vs, 100, 100, 110) * p.width;

         const double form = f1bp_(x, f1, f1 / b1, sr_) + g2 * f2bp_(x, f2, f2 / b2, sr_) + g3 * f3bp_(x, f3, f3 / b3, sr_);

         return dc_(form * p.gain + (k1.direct + k2.direct) * p.burst);
      }

   private:
      double ms2s(double ms) const { return ms * sr_ / 1000.0; }

      double sr_ = 44100.0, prevGate_ = 0.0;
      Noise noise_;
      Phasor osc_, sub_, fry_;
      RandomSah jitSah_, frySah_;
      Slide env_, jitSlide_, vowel_;
      SvfBP f1bp_, f2bp_, f3bp_;
      Consonant c1_, c2_;
      DcBlock dc_;
   };

} // namespace growl
