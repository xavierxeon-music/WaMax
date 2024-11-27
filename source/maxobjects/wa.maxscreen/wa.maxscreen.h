#ifndef MaxScreenH
#define MaxScreenH

#include "c74_min.h"
using namespace c74::min;

#include <mutex>

#include <QImage>
#include <QTcpSocket>

#include "ScreenSize.h"
#include "TouchPoint.h"

class MaxScreen : public object<MaxScreen>, public matrix_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   MaxScreen(const atoms& args = {});
   ~MaxScreen();

public:
   template <typename matrix_type>
   matrix_type calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position);

   pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position);

private:
   atoms timerFunction(const atoms& args, const int inlet);

   void sendData();
   void receiveData();

private:
   QTcpSocket* socket;
   QImage buffer;
   mutex bufferMutex;
   ScreenSize screenSize;
   TouchPoint::Map tpMap;

   inlet<> input;
   outlet<> output; // needs matrix output !
   attribute<symbol> hostName;
   timer<timer_options::defer_delivery> loopTimer;
};

#endif // MaxScreenH
