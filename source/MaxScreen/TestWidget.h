#ifndef TestWidgetH
#define TestWidgetH

#include "ui_TestWidget.h"
#include <QWidget>

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

   private:
      Client* client;
   };
} // namespace Test

#endif // NOT TestWidgetH
