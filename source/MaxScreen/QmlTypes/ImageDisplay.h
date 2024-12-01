#ifndef ImageDisplayH
#define ImageDisplayH

#include <QQmlEngine>
#include <QQuickPaintedItem>

#include "SharedImage.h"

class ImageDisplay : public QQuickPaintedItem
{
   Q_OBJECT
   QML_NAMED_ELEMENT(ImageDisplay)

public:
   ImageDisplay(QQuickItem* parent = nullptr);
   ~ImageDisplay();

public:
   void detach();
   void attach();

private:
   void paint(QPainter* painter) override;

private:
   SharedImage memorySubscriber;
   QImage image;
   bool isInitialised;
};

#endif // NOT ImageDisplayH
