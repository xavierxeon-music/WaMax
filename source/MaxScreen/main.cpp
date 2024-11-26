#include <QGuiApplication>
#include <QQmlApplicationEngine>

// main function

int main(int argc, char** argv)
{
   QGuiApplication app(argc, argv);

   QQmlApplicationEngine engine;
   engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
   if (engine.rootObjects().isEmpty())
      return -1;

   return app.exec();
}
