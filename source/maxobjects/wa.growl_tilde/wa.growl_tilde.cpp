#include "wa.growl_tilde.h"

#include <MaxPatcher.h>

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
      out[i] = m_voice.process(sigGate ? in[i] : m_gate, p);
   }
}

atoms Growl::Instance::numberFunction(const atoms& args, const int inlet)
{
   m_gate = static_cast<double>(args[0]) > 0.0 ? 1.0 : 0.0;
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
      m_gate = 1.0;
      m_note = static_cast<int>(pitch);
   }
   else if (static_cast<int>(pitch) == m_note)
   { // ignore stale note-offs (legato)
      m_gate = 0.0;
   }
   return {};
}

atoms Growl::Instance::setupFunction(const atoms& args, const int inlet)
{
   m_voice.set_samplerate(args[0]);
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
