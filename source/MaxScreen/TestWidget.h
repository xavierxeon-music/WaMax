#ifndef TestWidgetH
#define TestWidgetH

#include "ui_TestWidget.h"
#include <QWidget>

#include <QStandardItemModel>

#include "Client.h"

namespace Test
{
   class Widget : public QWidget, private Ui::Widget
   {
      Q_OBJECT

   public:
      Widget(QWidget* parent);

   private slots:
      void slotConnectToServer();
      void slotSelectImage();
      void slotSendImage();
      void slotSizeChanged(const ImageSize& imageSize);
      void slotTouchPointsChanged(const TouchPoint::Map& touchPointMap);

   private:
      Client* client;
      QStandardItemModel* touchModel;
   };
} // namespace Test

#endif // NOT TestWidgetH
