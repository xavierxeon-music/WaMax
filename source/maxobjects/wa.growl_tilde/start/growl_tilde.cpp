#include "c74_min.h"
#include "growl_dsp.h"

using namespace c74::min;

class growl_tilde : public object<growl_tilde>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"Death-metal growl synthesizer with vowels and consonants."};
   MIN_TAGS{"audio, synthesis, voice"};
   MIN_AUTHOR{"Ralf"};
   MIN_RELATED{"gen~"};

   inlet<> in_gate{this, "(signal/number) gate 0/1, (list) pitch velocity"};
   outlet<> out{this, "(signal) growl", "signal"};

   // --- attributes: settable with "name value" messages, like the gen~ params
   attribute<number> freq{this, "freq", 85.0, description{"Fundamental (Hz)"}};
   attribute<number> jitter{this, "jitter", 0.03, description{"Random pitch wobble"}};
   attribute<number> sub{this, "sub", 0.6, description{"Sub-octave level"}};
   attribute<number> noiseamt{this, "noiseamt", 0.5, description{"Breath noise level"}};
   attribute<number> fryrate{this, "fryrate", 35.0, description{"Rattle rate (Hz)"}};
   attribute<number> frydepth{this, "frydepth", 0.7, description{"Rattle amount"}};
   attribute<number> drive{this, "drive", 6.0, description{"Distortion"}};
   attribute<number> vowel{this, "vowel", 1.0, description{"0=U 1=O 2=A"}};
   attribute<number> shift{this, "shift", 0.9, description{"Formant scale"}};
   attribute<number> width{this, "width", 2.5, description{"Formant bandwidth multiplier"}};
   attribute<number> burst{this, "burst", 0.8, description{"Consonant level"}};
   attribute<int> cstart{this, "cstart", 0, description{"Consonant on note-on (0-10)"}};
   attribute<int> cend{this, "cend", 0, description{"Consonant on note-off (0-10)"}};
   attribute<number> att{this, "att", 8.0, description{"Attack (ms)"}};
   attribute<number> rel{this, "rel", 120.0, description{"Release (ms)"}};
   attribute<number> gain{this, "gain", 2.0, description{"Output gain"}};

   // --- gate as a plain number (when no signal is connected)
   message<> number{this, "number", "Gate 0/1",
                    MIN_FUNCTION{
                       m_gate = static_cast<double>(args[0]) > 0.0 ? 1.0 : 0.0;
   return {};
}
}
;

// --- "pitch velocity" straight from [notein] / [midiparse]
message<> list{this, "list", "pitch velocity",
               MIN_FUNCTION{
                  if (args.size() < 2) return {};
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
}
;

message<> dspsetup{this, "dspsetup",
                   MIN_FUNCTION{
                      m_voice.set_samplerate(args[0]);
return {};
}
}
;

void operator()(audio_bundle input, audio_bundle output)
{
   const growl::Params p = snapshot(); // read attributes once per vector
   const bool sigGate = in_gate.has_signal_connection();
   const double* in = input.samples(0);
   double* o = output.samples(0);

   for (auto i = 0; i < input.frame_count(); ++i)
      o[i] = m_voice.process(sigGate ? in[i] : m_gate, p);
}

private:
growl::Params snapshot() const
{
   growl::Params p;
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

growl::Voice m_voice;
double m_gate{0.0};
int m_note{-1};
}
;

MIN_EXTERNAL(growl_tilde);
