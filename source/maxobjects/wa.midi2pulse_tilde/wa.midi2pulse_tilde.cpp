#include "wa.midi2pulse_tilde.h"

#include <ByteFours.h>
#include <MaxPatcher.h>

MidiToPulse::MidiToPulse(const atoms& args)
   : object<MidiToPulse>()
   , vector_operator<>()
   , input{this, "(int) midi value"}
   , output{this, "(signal) pulse", "signal"}
   , intMessage{this, "int", "integer value.", Max::Patcher::minBind(this, &MidiToPulse::intFunction)}
   , buffer(10, 0)
   , bufferSize(0)
{
}

void MidiToPulse::operator()(audio_bundle inputs, audio_bundle outputs)
{
   static const int offsets[8] = {0, 1, 2, 3, 4, 5, 6, 7};
   uint16_t sendIndex = 0;

   double* out = outputs.samples(0);
   for (long frame = 0; frame < outputs.frame_count(); frame += 8)
   {
      const int index = frame / 8;
      if (index < bufferSize)
      {
         ByteFours bf(buffer[index]);

         out[frame + 0] = bf.value(ByteFours::FoursValue::O);
         out[frame + 1] = 0.0;
         out[frame + 2] = bf.value(ByteFours::FoursValue::I);
         out[frame + 3] = 0.0;
         out[frame + 4] = bf.value(ByteFours::FoursValue::Z);
         out[frame + 5] = 0.0;
         out[frame + 6] = bf.value(ByteFours::FoursValue::T);
         out[frame + 7] = 0.0;

         sendIndex++;
      }
      else
      {
         for (const int offset : offsets)
         {
            if (frame + offset < outputs.frame_count())
            {
               out[frame + offset] = 0.0;
            }
         }
      }
   }

   bufferSize -= sendIndex;

   std::copy(buffer.begin() + sendIndex, buffer.end(), buffer.begin());
   if (bufferSize < 10)
      buffer.resize(10);
}

atoms MidiToPulse::intFunction(const atoms& args, const int inlet)
{
   int value = args[0];
   if (value < 0)
      value = 0;
   else if (value > 255)
      value = 255;

   buffer[bufferSize++] = static_cast<uint8_t>(value);
   if (bufferSize > buffer.size())
      buffer.resize(buffer.size() + 10);

   return {};
}

MIN_EXTERNAL(MidiToPulse);
