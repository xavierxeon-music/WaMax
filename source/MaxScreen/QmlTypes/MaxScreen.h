#ifndef MaxScreenH
#define MaxScreenH

#include <QTcpServer>

#include <QImage>
#include <QPointer>
#include <QQmlEngine>
#include <QTcpSocket>
#include <QVector2D>

#include "Rainbow.h"

class MaxScreen : public QTcpServer
{
   Q_OBJECT
   QML_NAMED_ELEMENT(MaxScreen)
   QML_SINGLETON

   Q_PROPERTY(int stackId READ getStackId NOTIFY signalStackIdChanged)
   Q_PROPERTY(QColor bgColor READ getBgColor NOTIFY signalColorChanged)

public:
   struct TouchPoint
   {
      int pointId;
      bool pressed;
      double x;
      double y;
      double pressure;
      QVector2D velocity;
      double startX;
      double startY;

      using Map = QMap<int, TouchPoint>;
   };

public:
   MaxScreen(QObject* parent = nullptr); // needs default constructor for automatic creation via engine

signals:
   void signalStackIdChanged();
   void signalColorChanged();

public:
   int getStackId() const;
   QColor getBgColor() const;
   int getScreenHeight() const;
   void setScreenHeight(int value);

   Q_INVOKABLE QString getHostName() const;
   Q_INVOKABLE quint32 getPort() const;
   Q_INVOKABLE void setWindowSize(int width, int height);
   Q_INVOKABLE void touchPointsUpdated(const QList<QObject*>& touchPoints);

private slots:
   void slotNewConnection();
   void slotSocketClosed();
   void slotSetDisplay(const QImage& image);
   void slotResetDisplay();
   void slotChangeColor();

private:
   void sendWindowSize();
   void sendPackage(const QJsonObject& object);

private:
   QPointer<QTcpSocket> socket;
   int stackId;
   Rainbow rainbow;
   QSize screenSize;
};

#endif // NOT MaxScreenH
