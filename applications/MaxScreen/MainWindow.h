#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

#include <QQuickWidget>

#include "Server.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow();
   ~MainWindow();

public slots:
   void slotToggleFullScreen();

private:
   Server* server;
   QQuickWidget* view;
};

#endif // NOT MainWindowH
