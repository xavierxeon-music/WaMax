#include "wa.maxscreen.matrix.h"

#include <QImage>

#include <MaxPatcher.h>
#include <Shared.h>

#include "Streamable.h"

using namespace c74;
using ScreenServer = Shared<"MaxScreen">;

MaxScreenMatrix::MaxScreenMatrix(const atoms& args)
   : object<MaxScreenMatrix>()
   , matrix_operator<>(false)
   , memoryPublisher()
   , image()
   , input{this, "(matrix) Input", "matrix"}
   , output{this, "(matrix) output", "matrix"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenMatrix::doubleClickFunction)}
   , resizeTimer(this, Max::Patcher::minBind(this, &MaxScreenMatrix::resizeFunction))
{
   // args not working in matrix operator

   image = memoryPublisher.createWithCurrentSize();
   //cout << "MaxScreenMatrix INIT " << image.size().width() << " x " << image.size().height() << endl;

   resizeTimer.delay(100);
}

MaxScreenMatrix::~MaxScreenMatrix()
{
}

template <typename matrix_type>
matrix_type MaxScreenMatrix::calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position)
{
   return matrix_type{};
}

pixel MaxScreenMatrix::calc_cell(pixel input, const matrix_info& info, matrix_coord& position)
{
   const int x = position.x();
   const int y = position.y();
   if (x >= image.width() || y >= image.height())
      return pixel{};

   if (x < 0 || y < 0)
      return pixel{};

   const QRgb color = qRgb(input[red], input[green], input[blue]);
   QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(y));
   line[x] = color;

   return pixel{};
}

atoms MaxScreenMatrix::resizeFunction(const atoms& args, const int inlet)
{
   if (!memoryPublisher.sizeMatch()) [[unlikely]]
   {
      image = memoryPublisher.createWithCurrentSize();
      //cout << "MaxScreenMatrix RESIZE " << image.size().width() << " x " << image.size().height() << endl;
   }

   resizeTimer.delay(100);
   return {};
}

atoms MaxScreenMatrix::doubleClickFunction(const atoms& args, const int inlet)
{
   if (!ScreenServer::isServerActive())
   {
      cout << "start max screen" << endl;
      ScreenServer::startApplication();
   }
   image = memoryPublisher.createWithCurrentSize();

   return {};
}

MIN_EXTERNAL(MaxScreenMatrix);
