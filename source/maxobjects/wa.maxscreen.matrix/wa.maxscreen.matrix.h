#ifndef MaxScreenMatrixH
#define MaxScreenMatrixH

#include "c74_min.h"
using namespace c74::min;

#ifdef WIN_VERSION
#undef hypot
#endif

#include <mutex>

#include <QImage>
#include <QLocalSocket>

#include "ImageSize.h"
#include "SharedImage.h"

class MaxScreenMatrix : public object<MaxScreenMatrix>, public matrix_operator<>
{
public:
   MIN_DESCRIPTION{"send matrix to screen"};

public:
   MaxScreenMatrix(const atoms& args = {});
   ~MaxScreenMatrix();

public:
   template <typename matrix_type>
   matrix_type calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position);

   pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position);

private:
   atoms timerFunction(const atoms& args, const int inlet);

   void sendData();

private:
   QLocalSocket socket;
   SharedImage publisherMemory;
   QImage buffer;
   ImageSize screenSize;

   inlet<> input;
   outlet<> output; // needs matrix output !

   timer<timer_options::defer_delivery> loopTimer;
};

#endif // MaxScreenMatrixH
