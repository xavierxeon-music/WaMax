#include "TestWidget.h"

#include <QFileDialog>
#include <QImage>

Test::Widget::Widget(QWidget* parent)
   : QWidget(parent)
   , client(nullptr)
   , touchModel(nullptr)
{
   setupUi(this);

   client = new Client(this);
   connect(client, &Client::signalSizeChanged, this, &Widget::slotSizeChanged);
   connect(client, &Client::signalTouchPointsChanged, this, &Widget::slotTouchPointsChanged);

   connect(connectButton, &QAbstractButton::clicked, this, &Widget::slotConnectToServer);
   connect(imageSelectButton, &QAbstractButton::clicked, this, &Widget::slotSelectImage);
   connect(imageSendButton, &QAbstractButton::clicked, this, &Widget::slotSendImage);

   touchModel = new QStandardItemModel(this);
   const QStringList headerLabels = {"Id",
                                     "x",
                                     "y",
                                     "startX",
                                     "startY",
                                     "pressure",
                                     "rotation",
                                     "velocity",
                                     "area"};
   touchModel->setHorizontalHeaderLabels(headerLabels);
   for (int index = 0; index < 12; index++)
   {
      QList<QStandardItem*> rowList;
      for (int col = 0; col < headerLabels.size(); col++)
      {
         QStandardItem* item = new QStandardItem();
         item->setEditable(false);
         if (0 == col)
         {
            item->setCheckable(true);
            item->setText(QString::number(index));
         }

         rowList.append(item);
      }

      touchModel->invisibleRootItem()->appendRow(rowList);
   }

   touchPointView->setModel(touchModel);
   touchPointView->setRootIsDecorated(false);
   touchPointView->setAlternatingRowColors(true);

   for (int col = 0; col < headerLabels.size(); col++)
      touchPointView->resizeColumnToContents(col);

#if defined(__APPLE__)
   imageInfo->setText("/Users/waspe/Dropbox/Pictures/PainLogo.png");
#elif defined(WIN32)
   imageInfo->setText("C:/Users/waspe/Dropbox/Pictures/PainLogo.png");
#endif
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

void Test::Widget::slotTouchPointsChanged(const TouchPoint::Map& touchPointMap)
{
   for (TouchPoint::Map::const_iterator it = touchPointMap.constBegin(); it != touchPointMap.constEnd(); it++)
   {
      const TouchPoint& tp = it.value();
      const int row = it.key();

      QStandardItem* idItem = touchModel->invisibleRootItem()->child(row, 0);
      idItem->setCheckState(tp.isPressed() ? Qt::Checked : Qt::Unchecked);

      auto setText = [&](int col, const QString& text)
      {
         QStandardItem* item = touchModel->invisibleRootItem()->child(row, col);
         item->setText(text);
      };

      auto setNumber = [&](int col, const auto& value)
      {
         setText(col, QString::number(value));
      };

      setNumber(1, tp.getX());
      setNumber(2, tp.getY());
      setNumber(3, tp.getStartX());
      setNumber(4, tp.getStartY());

      setNumber(5, tp.getPressure());
      setNumber(6, tp.getRotation());

      QString velocity = QString("%1:%2").arg(tp.getVelocity().x()).arg(tp.getVelocity().y());
      setText(7, velocity);

      setNumber(8, tp.getArea());
   }
}
