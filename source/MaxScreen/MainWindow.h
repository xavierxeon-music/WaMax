#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

#include <QQuickWidget>

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow();

public slots:
   void slotToggleFullScreen();

private:
   QQuickWidget* view;
};

#endif // NOT MainWindowH
