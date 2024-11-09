#ifndef GraphWidgetH
#define GraphWidgetH

#include <QGraphicsView>

#include <QGraphicsScene>
#include <QJsonObject>

#include "GraphMaxPatch.h"

namespace Graph
{
   class Widget : public QGraphicsView, public Max::Patch
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent);

   public slots:
      void slotLoad(const QString& patchFileName);

   private:
      void wheelEvent(QWheelEvent* wheelEvent) override;
      void mouseDoubleClickEvent(QMouseEvent* mouseEvent) override;

      void keyPressEvent(QKeyEvent* event) override;
      void keyReleaseEvent(QKeyEvent* event) override;

      void updateZoom(bool save);

   private:
      QGraphicsScene* scene;

      QPen blackBorderPen;
      QPen blackLinePen;
      QPen blueLinePen;
      QBrush whiteBrush;
      QBrush blackBrush;
      QFont font;

      double zoomLevel;
   };
} // namespace Graph

#endif // NOT GraphWidgetH
