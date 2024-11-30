#include "wa.maxscreen.h"

#include <QBuffer>

#include <MaxPatcher.h>

#include "Convertor.h"

using namespace c74;

MaxScreen::MaxScreen(const atoms& args)
   : object<MaxScreen>()
   , matrix_operator<>(false)
   , socket(nullptr)
   , buffer(512, 512, QImage::Format_RGB32)
   , bufferMutex()
   , screenSize()
   , tpMap()
   , input{this, "(matrix) Input", "matrix"}
   , output{this, "(matrix) output", "matrix"}
   , hostName{this, "hostname", "127.0.0.1"}
   , loopTimer(this, Max::Patcher::minBind(this, &MaxScreen::timerFunction))
{
   socket = new QTcpSocket(nullptr);
   // args not working in matrix operator
   loopTimer.delay(10);
}

MaxScreen::~MaxScreen()
{
   socket->close();

   delete socket;
   socket = nullptr;
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
   //cout << x << "," << y << "," << color.red() << "," << color.green() << "," << color.blue() << endl;

   bufferMutex.lock();
   buffer.setPixelColor(x, y, color);
   bufferMutex.unlock();

   return pixel{};
}

atoms MaxScreen::timerFunction(const atoms& args, const int inlet)
{
   if (QTcpSocket::UnconnectedState == socket->state())
   {
      const std::string name = hostName.get();
      cout << "hostName: " << name << endl;
      socket->connectToHost(QString::fromStdString(name), 6667);
      socket->waitForConnected(10);
   }
   else if (QTcpSocket::ConnectedState == socket->state())
   {
      auto readyRead = [&]()
      {
         if (!socket->waitForReadyRead(10))
            return false;

         if (socket->bytesAvailable() == 0)
            return false;

         return true;
      };

      if (readyRead())
         receiveData();

      sendData();
      loopTimer.delay(100);
   }
   else
   {
      cout << "state: " << socket->state() << ", error: " << socket->error() << ", " << socket->errorString().toStdString() << endl;
   }

   loopTimer.delay(1000);

   return {};
}

void MaxScreen::sendData()
{
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
}

void MaxScreen::receiveData()
{
   QDataStream stream(socket);

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
      buffer.fill(QColor(255, 255, 0));
   }

   socket->readAll();
}

MIN_EXTERNAL(MaxScreen);
