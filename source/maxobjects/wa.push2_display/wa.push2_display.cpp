#include "wa.push2_display.h"

#include <map>

#include <patcher.h>

// see https://www.qt.io/blog/2015/12/15/ableton-push-qt-in-music-making
// see https://github.com/pixsperdavid/imp.push

static const int imageLength = 1024 * 160;
static const int dataLength = imageLength * 2; // RGB 16 image

Push2Display::Push2Display()
   : object<Push2Display>()
   , matrix_operator<>(false)
   , input{this, "(matrix) Input", "matrix"}
   , output{this, "(matrix) output", "matrix"}
   , updateTimer{this, Patcher::minBind(this, &Push2Display::timerFunction)}
   , context(nullptr)
   , device(nullptr)
   , bufferData(nullptr)
   , sendData(nullptr)
   , bufferMutex()
{
   libusb_init(&context);
   bindDevice();

   bufferData = new ushort[imageLength];
   std::memset(bufferData, 0, dataLength);
   defaultImage();

   sendData = new uchar[dataLength];
   updateTimer.delay(1000);
}

Push2Display::~Push2Display()
{
   unbindDevice();

   if (context)
      libusb_exit(context);

   delete[] bufferData;
   delete[] sendData;
}

template <typename matrix_type>
matrix_type Push2Display::calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position)
{
   return matrix_type{};
}

pixel Push2Display::calc_cell(pixel input, const matrix_info& info, matrix_coord& position)
{
   const int x = position.x();
   if (x < 0 || x >= 960)
      return pixel{};

   const int y = position.y();
   if (y < 0 || y >= 160)
      return pixel{};

   const ushort color = rgb16Color(input[red], input[green], input[blue]);
   setColor(x, y, color);

   return pixel{};
}

ushort Push2Display::rgb16Color(uchar red, uchar green, uchar blue) const
{
   const ushort r = (red / 8) << 0;   // max 32 colors = 5 bit -> shift by 0
   const ushort g = (green / 4) << 5; // max 64 colors = 6 bit -> shift by 5
   const ushort b = (blue / 8) << 11; // max 32 colors = 5 bit -> shift by 5 + 6

   const ushort color = r + g + b;
   return color;
}

void Push2Display::setColor(int x, int y, ushort color)
{
   const int index = x + (1024 * y);
   if (index >= imageLength)
      return;

   bufferMutex.lock();
   bufferData[index] = color;
   bufferMutex.unlock();
}

atoms Push2Display::timerFunction(const atoms& args, const int inlet)
{
   if (!bindDevice())
   {
      updateTimer.delay(1000);
      return {};
   }

   bufferMutex.lock();
   std::memcpy(sendData, bufferData, dataLength);
   bufferMutex.unlock();

   if (0 == transferBuffer())
      unbindDevice(); // device no longer available

   updateTimer.delay(100);

   return {};
}

bool Push2Display::bindDevice()
{
   if (device)
      return true;

   device = libusb_open_device_with_vid_pid(context, 0x2982, 0x1967);
   if (device)
      libusb_claim_interface(device, 0);

   return false;
}

void Push2Display::unbindDevice()
{
   if (!device)
      return;

   libusb_release_interface(device, 0);
   libusb_close(device);

   device = nullptr;
}

int Push2Display::transferBuffer()
{
   static const uchar endpoint = 0x01 | LIBUSB_ENDPOINT_OUT;
   static const uint timeout = 200;

   int transferred = 0;

   // header
   static uchar header[] = {0xEF, 0xCD, 0xAB, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   libusb_bulk_transfer(device, endpoint, header, sizeof(header), &transferred, timeout);

   // data
   libusb_bulk_transfer(device, endpoint, sendData, dataLength, &transferred, timeout);

   return transferred;
}

void Push2Display::defaultImage()
{
   using PaddingMap = std::map<int, ushort>;
   static const PaddingMap paddingMap = {
      {0, rgb16Color(0, 0, 0)},
      {10, rgb16Color(255, 0, 0)},
      {20, rgb16Color(255, 255, 0)},
      {30, rgb16Color(0, 255, 0)},
      {40, rgb16Color(0, 255, 255)},
      {50, rgb16Color(0, 0, 255)},
      {60, rgb16Color(255, 0, 255)},
      {70, rgb16Color(255, 255, 255)},
   };

   for (PaddingMap::const_iterator it = paddingMap.cbegin(); it != paddingMap.cend(); it++)
   {
      const int padding = it->first;
      const ushort color = it->second;
      for (int x = padding; x < 960 - padding; x++)
      {
         for (int y = padding; y < 160 - padding; y++)
         {
            setColor(x, y, color);
         }
      }
   }
}

MIN_EXTERNAL(Push2Display);