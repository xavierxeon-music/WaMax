#ifndef ImageDisplayH
#define ImageDisplayH

#include <QQmlEngine>
#include <QQuickPaintedItem>

#include <QImage>

class ImageDisplay : public QQuickPaintedItem
{
   Q_OBJECT
   QML_NAMED_ELEMENT(ImageDisplay)

public:
   ImageDisplay(QQuickItem* parent = nullptr);
   ~ImageDisplay();

public:
   void setBuffer(const QImage& image);

private:
   void paint(QPainter* painter) override;

private:
   QImage buffer;
};

#endif // NOT ImageDisplayH
