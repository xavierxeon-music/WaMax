#ifndef TestWidgetH
#define TestWidgetH

#include "ui_TestWidget.h"
#include <QWidget>

namespace Test
{
   class Client;

   class Widget : public QWidget, private Ui::Widget
   {
      Q_OBJECT

   public:
      Widget(QWidget* parent);

   private slots:
      void slotConnectToServer();
      void slotSelectImage();
      void slotSendImage();

   private:
      Client* client;
   };
} // namespace Test

#endif // NOT TestWidgetH
