#include "wa.growl_tilde.h"

#include <MaxPatcher.h>

constexpr double kPi = 3.14159265358979323846;

// ---------- building blocks (each mirrors a gen~ operator) ----------

Growl::Noise::Noise(uint32_t seed)
   : s(seed ? seed : 1)
{
}
double Growl::Noise::operator()()
{
   s ^= s << 13;
   s ^= s >> 17;
   s ^= s << 5;
   return s * (2.0 / 4294967296.0) - 1.0;
}

double Growl::Phasor::operator()(double hz, double sr)
{
   ph += hz / sr;
   wrapped = ph >= 1.0;
   if (wrapped)
      ph -= std::floor(ph);
   return ph;
}

double Growl::Slide::operator()(double x, double up, double down)
{
   const double n = std::max(x > y ? up : down, 1.0);
   y += (x - y) / n;
   return y;
}

double Growl::RandomSah::operator()(double hz, double sr, Noise& n)
{
   p(hz, sr);
   if (p.wrapped)
      held = n();
   return held;
}

double Growl::SvfBP::operator()(double x, double fc, double q, double sr)
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

double Growl::DcBlock::operator()(double x)
{
   const double y = x - x1 + 0.9997 * y1;
   x1 = x;
   y1 = y;
   return y;
}

inline double vowel3(double v, double u, double o, double a)
{
   return v < 1.0 ? u + (o - u) * v : o + (a - o) * (v - 1.0);
}

// ---------- consonants ----------

inline constexpr Growl::ConsonantSpec kConsonants[] = {
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

void Growl::Consonant::trigger(int idx)
{
   cur_ = std::clamp(idx, 0, kNumConsonants - 1);
   t_ = 0.0;
   rollPh_ = 0.0;
}

Growl::ConsonantOut Growl::Consonant::process(double sr, Noise& noise)
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
// ---------- voice ----------

Growl::Voice::Voice(uint32_t seed)
   : noise_(seed)
{
}
void Growl::Voice::set_samplerate(double sr)
{
   sr_ = sr;
}

double Growl::Voice::process(double gate, const Params& p)
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

double Growl::Voice::ms2s(double ms) const
{
   return ms * sr_ / 1000.0;
}

Growl::Instance::Instance(const atoms& args)
   : object<Growl::Instance>()
   , in_gate{this, "(signal/number) gate 0/1, (list) pitch velocity"}
   , out{this, "(signal) growl", "signal"}
   , freq{this, "freq", 85.0, description{"Fundamental (Hz)"}}
   , jitter{this, "jitter", 0.03, description{"Random pitch wobble"}}
   , sub{this, "sub", 0.6, description{"Sub-octave level"}}
   , noiseamt{this, "noiseamt", 0.5, description{"Breath noise level"}}
   , fryrate{this, "fryrate", 35.0, description{"Rattle rate (Hz)"}}
   , frydepth{this, "frydepth", 0.7, description{"Rattle amount"}}
   , drive{this, "drive", 6.0, description{"Distortion"}}
   , vowel{this, "vowel", 1.0, description{"0=U 1=O 2=A"}}
   , shift{this, "shift", 0.9, description{"Formant scale"}}
   , width{this, "width", 2.5, description{"Formant bandwidth multiplier"}}
   , burst{this, "burst", 0.8, description{"Consonant level"}}
   , cstart{this, "cstart", 0, description{"Consonant on note-on (0-10)"}}
   , cend{this, "cend", 0, description{"Consonant on note-off (0-10)"}}
   , att{this, "att", 8.0, description{"Attack (ms)"}}
   , rel{this, "rel", 120.0, description{"Release (ms)"}}
   , gain{this, "gain", 2.0, description{"Output gain"}}
   , number{this, "number", "Gate 0/1", Max::Patcher::minBind(this, &Instance::numberFunction)}
   , list{this, "list", "pitch velocity", Max::Patcher::minBind(this, &Instance::listFunction)}
   , dspsetup{this, "dspsetup", Max::Patcher::minBind(this, &Instance::setupFunction)}
{
}

void Growl::Instance::operator()(audio_bundle input, audio_bundle output)
{
   const Params p = snapshot(); // read attributes once per vector
   const bool sigGate = in_gate.has_signal_connection();
   const double* in = input.samples(0);
   double* out = output.samples(0);

   for (int i = 0; i < input.frame_count(); ++i)
   {
      out[i] = voice.process(sigGate ? in[i] : gate, p);
   }
}

atoms Growl::Instance::numberFunction(const atoms& args, const int inlet)
{
   gate = static_cast<double>(args[0]) > 0.0 ? 1.0 : 0.0;
   return {};
}

atoms Growl::Instance::listFunction(const atoms& args, const int inlet)
{
   if (args.size() < 2)
      return {};
   const double pitch = args[0];
   const double vel = args[1];
   if (vel > 0)
   {
      freq = 440.0 * std::pow(2.0, (pitch - 69.0) / 12.0);
      gate = 1.0;
      note = static_cast<int>(pitch);
   }
   else if (static_cast<int>(pitch) == note)
   { // ignore stale note-offs (legato)
      gate = 0.0;
   }
   return {};
}

atoms Growl::Instance::setupFunction(const atoms& args, const int inlet)
{
   voice.set_samplerate(args[0]);
   return {};
}

Growl::Params Growl::Instance::snapshot() const
{
   Params p;
   p.freq = freq;
   p.jitter = jitter;
   p.sub = sub;
   p.noiseamt = noiseamt;
   p.fryrate = fryrate;
   p.frydepth = frydepth;
   p.drive = drive;
   p.vowel = vowel;
   p.shift = shift;
   p.width = width;
   p.burst = burst;
   p.cstart = cstart;
   p.cend = cend;
   p.att = att;
   p.rel = rel;
   p.gain = gain;
   return p;
}

MIN_EXTERNAL(Growl::Instance);
