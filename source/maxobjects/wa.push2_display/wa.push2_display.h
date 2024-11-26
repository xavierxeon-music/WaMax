#ifndef Push2DisplayH
#define Push2DisplayH

#include <mutex>

#include <libusb.h>

#include "c74_min.h"
using namespace c74::min;

class Push2Display : public object<Push2Display>, public matrix_operator<>
{
public:
   MIN_DESCRIPTION{"Display matrix on Ableton Push2 display"};

public:
   Push2Display();
   ~Push2Display();

public:
   template <typename matrix_type>
   matrix_type calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position);

   pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position);

private:
   bool bindDevice();
   void unbindDevice();
   ushort rgb16Color(uchar red, uchar green, uchar blue) const;
   void setColor(int x, int y, ushort color);
   atoms timerFunction(const atoms& args, const int inlet);
   int transferBuffer();
   void defaultImage();

private:
   libusb_context* context;
   libusb_device_handle* device;
   ushort* bufferData;
   uchar* sendData;
   mutex bufferMutex;

   inlet<> input;
   outlet<> output;  // needs matrix output !
   outlet<> output2; // needs matrix output !
   timer<timer_options::defer_delivery> updateTimer;
};

#endif // NOT WaPush2DisplayH