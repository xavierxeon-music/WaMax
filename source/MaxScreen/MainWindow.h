#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow();

private slots:
   void slotToggleFullScreen();
};

#endif // NOT MainWindowH
