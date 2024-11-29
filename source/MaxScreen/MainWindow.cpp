#include "MainWindow.h"

#include <QApplication>
#include <QQmlEngine>
#include <QQuickWidget>

MainWindow::MainWindow()
   : QMainWindow(nullptr)
{
#ifdef Q_OS_WIN
   // anything else will make QQUickWidget crash
   QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif

   QQuickWidget* view = new QQuickWidget(this);
   view->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
   view->setResizeMode(QQuickWidget::ResizeMode::SizeRootObjectToView);
   view->setSource(QUrl("qrc:/MainWindow.qml"));

   setCentralWidget(view);
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   MainWindow mw;
   mw.show();

   return app.exec();
}
