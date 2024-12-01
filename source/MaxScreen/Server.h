#ifndef ServerH
#define ServerH

#include <QLocalServer>

#include <QImage>
#include <QLocalSocket>
#include <QPointer>
#include <QQmlEngine>
#include <QVector2D>

#include "Data.h"
#include "ImageDisplay.h"
#include "Rainbow.h"

class Server : public QLocalServer, private Data
{
   Q_OBJECT

   Q_PROPERTY(int stackId READ getStackId NOTIFY signalStackIdChanged)
   Q_PROPERTY(QColor bgColor READ getBgColor NOTIFY signalColorChanged)

public:
   Server(QObject* parent);

signals:
   void signalStackIdChanged();
   void signalColorChanged();
   void signalToolgeFullScreen();

public:
   int getStackId() const;
   QColor getBgColor() const;
   int getScreenHeight() const;
   void setScreenHeight(int value);

   Q_INVOKABLE void setWindowSize(int width, int height);
   Q_INVOKABLE void touchPointsUpdated(const QList<QObject*>& touchPoints);
   Q_INVOKABLE void toogleFullScreen();
   Q_INVOKABLE void setImageDisplay(QObject* displayObject);

private slots:
   void slotNewConnection();
   void slotSocketClosed();
   void slotSocketRead();
   void slotChangeColor();
   void sendTouchPointUpdates();

private:
   void sendWindowSize();

private:
   QPointer<QLocalSocket> socket;
   int stackId;
   Rainbow rainbow;
   ImageDisplay* imageDisplay;
   QByteArray imageBuffer;
   qsizetype imageSize;
};

#endif // NOT ServerH
