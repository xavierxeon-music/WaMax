#ifndef MainWindowH
#define MainWindowH

#include "ui_MainWindow.h"
#include <QWidget>

#include <QTcpSocket>

#include "ScreenSize.h"
#include "TouchPoint.h"

class MainWindow : public QWidget, private Ui::MainWindow
{
   Q_OBJECT

public:
   MainWindow();
   ~MainWindow();

private slots:
   void slotTryConnect();
   void slotStateChanged(QAbstractSocket::SocketState socketState);
   void slotReadData();
   void slotSelectImage();
   void slotSendImage();

private:
   QTcpSocket* socket;
   bool local;
   ScreenSize screenSize;
   TouchPoint::Map tpMap;
};

#endif // NOT MainWindowH
