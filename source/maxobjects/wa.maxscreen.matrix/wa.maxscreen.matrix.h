#ifndef MaxScreenMatrixH
#define MaxScreenMatrixH

// keep order
#include "c74_min.h"
using namespace c74::min;
#include <CommonQt.h>

#include <QImage>

#include "SharedImage.h"

class MaxScreenMatrix : public object<MaxScreenMatrix>, public matrix_operator<>
{
public:
   MIN_DESCRIPTION{"matrix from / to screen"};

public:
   MaxScreenMatrix(const atoms& args = {});

public:
   template <typename matrix_type>
   matrix_type calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position);

   pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position);

private:
   atoms resizeFunction(const atoms& args, const int inlet);

private:
   // send matrix to gui
   inlet<> input;
   SharedImage memoryPublisher;
   QImage inputImage;

   // get scrrenshot from gui
   outlet<> output;
   SharedImage memorySubscriber;
   QImage outputImage;

   timer<timer_options::defer_delivery> resizeTimer;
};

#endif // MaxScreenMatrixH
