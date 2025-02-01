#ifndef MaxScreenMatrixH
#define MaxScreenMatrixH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include "ScreenClient.h" // base class

#include <QImage>

#include "ImageSize.h"
#include "SharedImage.h"

//

class MaxScreenMatrix : public object<MaxScreenMatrix>, public matrix_operator<>, public ScreenClient
{
public:
   MIN_DESCRIPTION{"send matrix to screen"};

public:
   MaxScreenMatrix(const atoms& args = {});

public:
   template <typename matrix_type>
   matrix_type calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position);

   pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position);

private:
   atoms doubleClickFunction(const atoms& args, const int inlet);
   void receiveData(QDataStream& stream) override;

private:
   SharedImage memoryPublisher;
   QImage image;
   ImageSize screenSize;

   inlet<> input;
   outlet<> output; // needs matrix output !
   message<> doubleClickMessage;
};

#endif // MaxScreenMatrixH
