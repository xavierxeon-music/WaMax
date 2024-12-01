#include "wa.maxscreen.matrix.h"

#include <QBuffer>

#include <MaxPatcher.h>

#include "Convertor.h"

using namespace c74;

// see https://doc.qt.io/qt-6/qtcore-ipc-sharedmemory-example.html

MaxScreenMatrix::MaxScreenMatrix(const atoms& args)
   : object<MaxScreenMatrix>()
   , matrix_operator<>(false)
   , socket()
   , buffer(512, 512, QImage::Format_RGB32)
   , input{this, "(matrix) Input", "matrix"}
   , output{this, "(matrix) output", "matrix"}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreenMatrix::timerFunction))
{
   // args not working in matrix operator
   loopTimer.delay(10);
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
   if (x < 0 || x >= buffer.width() || y < 0 || y >= buffer.height())
      return pixel{};

   const QColor color(input[red], input[green], input[blue]);
   //cout << x << "," << y << "," << color.red() << "," << color.green() << "," << color.blue() << endl;

   buffer.setPixelColor(x, y, color);

   return pixel{};
}

atoms MaxScreenMatrix::timerFunction(const atoms& args, const int inlet)
{
   /*
   if (QLocalSocket::UnconnectedState == socket.state())
      socket.connectToServer(HelpForMax::compileSocketName());

   if (PatchInfo::State::Initial != currentPatch.state || PatchInfo::State::NotInPackage != currentPatch.state)
   {
      auto readyRead = [&]()
      {
         if (socket.state() != QLocalSocket::ConnectedState)
            return false;

         if (!socket.waitForReadyRead(10))
            return false;

         if (socket.bytesAvailable() == 0)
            return false;

         return true;
      };

      if (readyRead())
         receiveData();
   }


   loopTimer.delay(500);
   */
   return {};
}

void MaxScreenMatrix::sendData()
{
   /*
   bufferMutex.lock();
   QByteArray block;
   {
      QBuffer writer(&block);
      buffer.save(&writer, "PNG");
   }
   bufferMutex.unlock();

   Convertor<qsizetype> convertor;
   convertor.data = block.size();
   //cout << "image size " << convertor.data << endl;

   socket->write(convertor.bytes, sizeof(qsizetype));
   socket->write(block);

   socket->waitForBytesWritten(10);
   */
}

MIN_EXTERNAL(MaxScreenMatrix);
