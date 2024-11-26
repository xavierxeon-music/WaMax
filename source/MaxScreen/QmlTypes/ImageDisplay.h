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
   static void push(const QImage& image);

private:
   void setBuffer(const QImage& image);
   void paint(QPainter* painter) override;

private:
   static ImageDisplay* me;
   QImage buffer;
};

#endif // NOT ImageDisplayH
