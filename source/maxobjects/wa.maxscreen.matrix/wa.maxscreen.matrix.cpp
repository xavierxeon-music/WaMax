#include "wa.maxscreen.matrix.h"

#include <Qimage>

#include <MaxPatcher.h>
#include <Shared.h>

using namespace c74;
using ScreenServer = Shared<"MaxScreen">;

MaxScreenMatrix::MaxScreenMatrix(const atoms& args)
   : object<MaxScreenMatrix>()
   , matrix_operator<>(false)
   , socket()
   , publisherMemory(true)
   , image(512, 512, QImage::Format_ARGB32)
   , screenSize()
   , input{this, "(matrix) Input", "matrix"}
   , output{this, "(matrix) output", "matrix"}
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenMatrix::doubleClickFunction)}
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
   if (x < 0 || x >= image.width() || y < 0 || y >= image.height())
      return pixel{};

   const QColor color(input[red], input[green], input[blue]);
   //cout << x << "," << y << "," << color.red() << "," << color.green() << "," << color.blue() << endl;

   image.setPixelColor(x, y, color);

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

atoms MaxScreenMatrix::timerFunction(const atoms& args, const int inlet)
{
   if (QLocalSocket::UnconnectedState == socket.state())
   {
      socket.connectToServer(ScreenServer::socketName());
      loopTimer.delay(500);
   }
   else if (QLocalSocket::ConnectedState == socket.state())
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

      socket.write(Marker::Image);
      loopTimer.delay(100);
   }
   else
   {
      loopTimer.delay(500);
   }

   return {};
}

void MaxScreenMatrix::receiveData()
{
   QDataStream stream(&socket);

   char marker;
   stream >> marker;

   if ('s' == marker)
   {
      screenSize.load(stream);
      image = memoryPublisher.createNew(screenSize.getWidth(), screenSize.getHeight());
      image.fill(QColor(0, 0, 0, 0));
   }

   socket.readAll();
}

MIN_EXTERNAL(MaxScreenMatrix);
