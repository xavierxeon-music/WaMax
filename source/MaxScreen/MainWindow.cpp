#include "MainWindow.h"

#include <QApplication>
#include <QDockWidget>
#include <QMenuBar>
#include <QQmlContext>
#include <QQmlEngine>
#include <QToolBar>

#include "ScreenServer.h"
#include "Test/TestWidget.h"

MainWindow::MainWindow()
   : QMainWindow(nullptr)
   , view(nullptr)
{
#ifdef Q_OS_WIN
   // anything else will make QQUickWidget crash
   QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif

   //QToolBar* testBar = addToolBar("Test");
   //testBar->addAction("Test Action");

   view = new QQuickWidget(this);
   view->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
   view->setResizeMode(QQuickWidget::ResizeMode::SizeRootObjectToView);
   view->setSource(QUrl("qrc:/MainWindow.qml"));

   setCentralWidget(view);

   auto createDockWidget = [&](QWidget* widget, const Qt::DockWidgetArea& area)
   {
      QDockWidget* dock = new QDockWidget(this);
      dock->setWidget(widget);
      dock->setTitleBarWidget(new QWidget());
      dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
      addDockWidget(area, dock);
   };

   Test::Widget* testWidget = new Test::Widget(this);
   createDockWidget(testWidget, Qt::LeftDockWidgetArea);

   ScreenServer* instance = view->engine()->singletonInstance<ScreenServer*>("ScreenServer", "ScreenServer");
   connect(instance, &ScreenServer::signalToolgeFullScreen, this, &MainWindow::slotToggleFullScreen);
}

void MainWindow::slotToggleFullScreen()
{
   if (isFullScreen())
      showMaximized();
   else
      showFullScreen();

   const QList<QToolBar*> toolBarList = findChildren<QToolBar*>();
   for (QToolBar* toolBar : toolBarList)
      toolBar->setVisible(!isFullScreen());

   menuBar()->setVisible(!isFullScreen());

   const QList<QDockWidget*> dockList = findChildren<QDockWidget*>();
   for (QDockWidget* dock : dockList)
      dock->setVisible(!isFullScreen());
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   MainWindow mw;
   mw.showMaximized();
   //mw.slotToggleFullScreen();

   return app.exec();
}
