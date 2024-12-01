#include "TestWidget.h"

#include <QFileDialog>
#include <QImage>

Test::Widget::Widget(QWidget* parent)
   : QWidget(parent)
   , client(nullptr)
{
   setupUi(this);

   client = new Client(this);
   connect(client, &Client::signalSizeChanged, this, &Widget::slotSizeChanged);

   connect(connectButton, &QAbstractButton::clicked, this, &Widget::slotConnectToServer);
   connect(imageSelectButton, &QAbstractButton::clicked, this, &Widget::slotSelectImage);
   connect(imageSendButton, &QAbstractButton::clicked, this, &Widget::slotSendImage);

   imageInfo->setText("/Users/waspe/Dropbox/Pictures/Danger.png");
}

void Test::Widget::slotConnectToServer()
{
   client->connectToServer();
}

void Test::Widget::slotSelectImage()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "Image");
   if (fileName.isEmpty())
      return;

   imageInfo->setText(fileName);
}

void Test::Widget::slotSendImage()
{
   const QString& fileName = imageInfo->text();
   if (fileName.isEmpty())
      return;

   client->sendImage(fileName);
}

void Test::Widget::slotSizeChanged(const ImageSize& imageSize)
{
   widthSpin->setValue(imageSize.getWidth());
   heightSpin->setValue(imageSize.getHeight());
}
