#include "MainWindow.h"

#include <QApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>

#include "ScreenServer.h"

MainWindow::MainWindow()
   : QMainWindow(nullptr)
{
#ifdef Q_OS_WIN
   // anything else will make QQUickWidget crash
   QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif

   addToolBar("Test");

   QQuickWidget* view = new QQuickWidget(this);
   view->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
   view->setResizeMode(QQuickWidget::ResizeMode::SizeRootObjectToView);
   view->setSource(QUrl("qrc:/MainWindow.qml"));

   setCentralWidget(view);

   ScreenServer* instance = view->engine()->singletonInstance<ScreenServer*>("ScreenServer", "ScreenServer");
   connect(instance, &ScreenServer::signalToolgeFullScreen, this, &MainWindow::slotToggleFullScreen);
}

void MainWindow::slotToggleFullScreen()
{
   if (isFullScreen())
      showMaximized();
   else
      showFullScreen();
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   MainWindow mw;
   mw.showMaximized();
   mw.showFullScreen();

   return app.exec();
}
