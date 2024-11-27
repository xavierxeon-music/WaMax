#include "wa.maxscreen.h"

#include <QDataStream>

#include <MaxPatcher.h>

MaxScreen::MaxScreen(const atoms& args)
   : object<MaxScreen>()
   , matrix_operator<>(false)
   , socket()
   , hostName("127.0.0.1")
   , buffer(512, 512, QImage::Format_RGB32)
   , bufferMutex()
   , screenSize()
   , tpMap()
   , input{this, "(matrix) Input", "matrix"}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreen::timerFunction))
{
   if (args.size() > 0)
   {
      const std::string text = args[0];
      hostName = QString::fromStdString(text);
   }

   loopTimer.delay(10);
}

template <typename matrix_type>
matrix_type MaxScreen::calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position)
{
   return matrix_type{};
}

pixel MaxScreen::calc_cell(pixel input, const matrix_info& info, matrix_coord& position)
{
   const int x = position.x();
   const int y = position.y();
   if (x < 0 || x >= buffer.width() || y < 0 || y >= buffer.height())
      return pixel{};

   const QColor color(input[red], input[green], input[blue]);

   bufferMutex.lock();
   buffer.setPixelColor(x, y, color);
   bufferMutex.unlock();

   return pixel{};
}

atoms MaxScreen::timerFunction(const atoms& args, const int inlet)
{
   if (QTcpSocket::UnconnectedState == socket.state())
   {
      socket.connectToHost(hostName, 6667);
   }
   else if (QTcpSocket::ConnectedState == socket.state())
   {
      auto readyRead = [&]()
      {
         if (!socket.waitForReadyRead(10))
            return false;

         if (socket.bytesAvailable() == 0)
            return false;

         return true;
      };

      if (readyRead())
         receiveData();

      sendData();
   }

   loopTimer.delay(500);
   return {};
}

void MaxScreen::sendData()
{
   QDataStream stream(&socket);

   bufferMutex.lock();
   stream << buffer;
   bufferMutex.unlock();
}

void MaxScreen::receiveData()
{
   QDataStream stream(&socket);

   char marker;
   stream >> marker;

   if ('t' == marker)
   {
      tpMap.load(stream);
   }
   else if ('s' == marker)
   {
      screenSize.load(stream);
      buffer = QImage(screenSize.getWidth(), screenSize.getHeight(), QImage::Format_RGB32);
   }

   socket.readAll();
}

MIN_EXTERNAL(MaxScreen);
