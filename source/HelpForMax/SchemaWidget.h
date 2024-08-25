#ifndef SchemaWidgetH
#define SchemaWidgetH

#include <QGraphicsView>

#include <QGraphicsScene>
#include <QJsonObject>

namespace Schema
{
   class Widget : public QGraphicsView
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent);

   public slots:
      void slotLoad(const QString& patchFileName);

   private:
      struct Box
      {
         QGraphicsRectItem* rectItem = nullptr;
         QGraphicsItem* foregroundItem = nullptr;
         int inletCount = 0;
         int outletCount = 0;
      };

      enum DataKeys
      {
         KeyHelp = 1
      };

      using IdMap = QMap<QString, Box>;

   private:
      IdMap makeObjects(const QJsonObject patcherObject);
      void makeLines(const QJsonObject patcherObject, const IdMap& idMap);
      void moveItems(const IdMap& idMap);

   private:
      QGraphicsScene* scene;
      QString patchFileName;

      QPen blackPen;
      QBrush whiteBrush;
      QBrush grayBrush;
      QFont font;
   };
} // namespace Schema

#endif // NOT SchemaWidgetH
