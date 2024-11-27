#ifndef ScreenServerH
#define ScreenServerH

#include <QTcpServer>

#include <QImage>
#include <QPointer>
#include <QQmlEngine>
#include <QTcpSocket>
#include <QVector2D>

#include "Rainbow.h"
#include "ScreenSize.h"
#include "TouchPoint.h"

class ScreenServer : public QTcpServer
{
   Q_OBJECT
   QML_NAMED_ELEMENT(ScreenServer)
   QML_SINGLETON

   Q_PROPERTY(int stackId READ getStackId NOTIFY signalStackIdChanged)
   Q_PROPERTY(QColor bgColor READ getBgColor NOTIFY signalColorChanged)

public:
   ScreenServer(QObject* parent = nullptr); // needs default constructor for automatic creation via engine

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
   void sendTouchPointUpdates();

private:
   void sendWindowSize();

private:
   QPointer<QTcpSocket> socket;
   int stackId;
   Rainbow rainbow;
   ScreenSize screenSize;
   TouchPoint::Map tpMap;
};

#endif // NOT ScreenServerH
