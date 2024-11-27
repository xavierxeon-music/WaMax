#include "MainWindow.h"

#include <QApplication>
#include <QBuffer>
#include <QFileDialog>
#include <QTimer>

#include <Convertor.h>

MainWindow::MainWindow()
   : QWidget(nullptr)
   , socket(nullptr)
   , local(false)
   , screenSize()
   , tpMap()
{
   setupUi(this);
   setWindowTitle("Max Screeen Test");

   local = (QApplication::instance()->arguments().count() > 1);
   socket = new QTcpSocket(this);

   connect(socket, &QTcpSocket::stateChanged, this, &MainWindow::slotStateChanged);
   connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadData);

   slotTryConnect();

   connect(imageSelectButton, &QAbstractButton::clicked, this, &MainWindow::slotSelectImage);
   connect(imageSendButton, &QAbstractButton::clicked, this, &MainWindow::slotSendImage);

   imagePathInfo->setText("/Users/waspe/Pictures/other/AspEKrkCMAAFvNY.jpg");
}

MainWindow::~MainWindow()
{
   disconnect(socket, &QTcpSocket::stateChanged, this, &MainWindow::slotStateChanged);
   disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadData);
}

void MainWindow::slotTryConnect()
{
   socket->connectToHost(local ? "127.0.0.1" : "192.168.1.106", 6667);
}

void MainWindow::slotStateChanged(QAbstractSocket::SocketState socketState)
{
   connectedCheck->setChecked(QAbstractSocket::ConnectedState == socketState);

   if (QAbstractSocket::UnconnectedState != socketState)
      return;

   QTimer::singleShot(5000, this, &MainWindow::slotTryConnect);
}

void MainWindow::slotReadData()
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
      widthInfo->setText(QString::number(screenSize.getWidth()));
      heightInfo->setText(QString::number(screenSize.getHeight()));
   }

   socket->readAll();
}

void MainWindow::slotSelectImage()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "Image");
   if (fileName.isEmpty())
      return;

   imagePathInfo->setText(fileName);
}

void MainWindow::slotSendImage()
{
   QImage image(imagePathInfo->text());
   if (image.isNull())
      return;

   QByteArray block;
   {
      QBuffer buffer(&block);
      image.save(&buffer, "PNG");
   }

   Convertor<qsizetype> convertor;
   convertor.data = block.size();

   socket->write(convertor.bytes, sizeof(qsizetype));
   socket->write(block);

   qDebug() << block.size();
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   MainWindow mw;
   mw.show();

   return app.exec();
}
