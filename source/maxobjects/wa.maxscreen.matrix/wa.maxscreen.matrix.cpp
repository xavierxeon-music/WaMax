#include "wa.maxscreen.matrix.h"

#include <QImage>

#include <MaxPatcher.h>
#include <Shared.h>

#include "Marker.h"

using namespace c74;
using ScreenServer = Shared<"MaxScreen">;

MaxScreenMatrix::MaxScreenMatrix(const atoms& args)
   : object<MaxScreenMatrix>()
   , matrix_operator<>(false)
   , ScreenClient()
   , memoryPublisher()
   , image(512, 512, QImage::Format_ARGB32)
   , screenSize()
   , input{this, "(matrix) Input", "matrix"}
   , output{this, "(matrix) output", "matrix"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenMatrix::doubleClickFunction)}
{
   // args not working in matrix operator
   startConnection();
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
   //image.setPixel(x, y, color);

   return pixel{};
}

atoms MaxScreenMatrix::doubleClickFunction(const atoms& args, const int inlet)
{
   if (!ScreenServer::isServerActive())
   {
      cout << "start max screen" << endl;
      ScreenServer::startApplication();
   }
   return {};
}

void MaxScreenMatrix::receiveData(QDataStream& stream)
{
   char marker;
   stream >> marker;

   if (Marker::ScreenSize == marker)
   {
      screenSize.load(stream);
      image = memoryPublisher.createWithCurrentSize();
      image.fill(QColor(0, 0, 0, 0));
   }
}

MIN_EXTERNAL(MaxScreenMatrix);
