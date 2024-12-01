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

private:
   void paint(QPainter* painter) override;

private:
   SharedImage subscriberImage;
};

#endif // NOT ImageDisplayH
