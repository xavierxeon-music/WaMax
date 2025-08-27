#include "jit.wa.maxscreen.h"

#include <MaxPatcher.h>
#include <Shared.h>

#include "Streamable.h"

using namespace c74;

JitMaxScreen::JitMaxScreen(const atoms& args)
   : object<JitMaxScreen>()
   , matrix_operator<>(false)
   , inputMatrix{this, "(matrix) Input", "matrix"}
   , memoryPublisher("input")
   , inputImage()
   , outputMatrix{this, "(matrix) output", "matrix"}
   , memorySubscriber("output")
   , outputImage()
   , resizeTimer(this, Max::Patcher::minBind(this, &JitMaxScreen::resizeFunction))
{
   // args not working in matrix operator

   inputImage = memoryPublisher.createWithCurrentSize();
   outputImage = memorySubscriber.createWithCurrentSize();
   //cout << "JitMaxScreen INIT " << inputImage.size().width() << " x " << inputImage.size().height() << endl;

   resizeTimer.delay(100);
}

template <typename matrix_type>
matrix_type JitMaxScreen::calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position)
{
   return matrix_type{};
}

pixel JitMaxScreen::calc_cell(pixel input, const matrix_info& info, matrix_coord& position)
{
   const int x = position.x();
   const int y = position.y();

   if (x < 0 || y < 0)
      return pixel{};

   if (x < inputImage.width() && y < inputImage.height())
   {
      const QRgb color = qRgb(input[red], input[green], input[blue]);
      QRgb* line = reinterpret_cast<QRgb*>(inputImage.scanLine(y));
      line[x] = color;
   }

   if (x >= outputImage.width() || y >= outputImage.height())
      return pixel{};

   QRgb* line = reinterpret_cast<QRgb*>(outputImage.scanLine(y));
   const QRgb color = line[x];

   pixel output;

   output[alpha] = 255;
   output[red] = static_cast<uchar>(255 * qRed(color));
   output[green] = static_cast<uchar>(255 * qGreen(color));
   output[blue] = static_cast<uchar>(255 * qBlue(color));

   return output;
}

atoms JitMaxScreen::resizeFunction(const atoms& args, const int inlet)
{
   if (!memoryPublisher.sizeMatch()) [[unlikely]]
   {
      inputImage = memoryPublisher.createWithCurrentSize();
      //cout << "JitMaxScreen INPUT RESIZE " << inputImage.size().width() << " x " << inputImage.size().height() << endl;
   }

   if (!memorySubscriber.sizeMatch()) [[unlikely]]
   {
      outputImage = memorySubscriber.createWithCurrentSize();
      //cout << "JitMaxScreen OUTPUT RESIZE " << outputImage.size().width() << " x " << outputImage.size().height() << endl;
   }

   resizeTimer.delay(100);
   return {};
}

MIN_EXTERNAL(JitMaxScreen);
