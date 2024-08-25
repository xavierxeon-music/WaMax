#include "SchemaWidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QGraphicsItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMouseEvent>

Schema::Widget::Widget(QWidget* parent)
   : QGraphicsView(parent)
   , scene(nullptr)
   , patchFileName()
   , blackPen(Qt::black)
   , whiteBrush(Qt::white)
   , grayBrush(QColor(230, 230, 230))
   , font()

{
   font.setPixelSize(10);

   scene = new QGraphicsScene(this);
   scene->setBackgroundBrush(whiteBrush);

   setScene(scene);
   setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void Schema::Widget::slotLoad(const QString& patchFileName)
{
   scene->clear();
   this->patchFileName = patchFileName;

   if (patchFileName.isEmpty())
      return;

   QFile file(patchFileName);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
   file.close();

   QJsonObject object = doc.object();
   if (object.empty())
      return;

   const QJsonObject patcherObject = object["patcher"].toObject();

   IdMap idMap = makeObjects(patcherObject);
   moveItems(idMap);
   makeLines(patcherObject, idMap);
}

Schema::Widget::IdMap Schema::Widget::makeObjects(const QJsonObject patcherObject)
{
   static const QStringList skipList = {"comment", "panel"};
   const QJsonArray boxArray = patcherObject["boxes"].toArray();

   IdMap idMap;

   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if (skipList.contains(className))
         continue;

      QJsonArray patchRectData = boxObject["patching_rect"].toArray();
      QRectF patchRect;
      patchRect.setX(patchRectData[0].toDouble());
      patchRect.setY(patchRectData[1].toDouble());
      patchRect.setWidth(patchRectData[2].toDouble());
      patchRect.setHeight(patchRectData[3].toDouble());

      QGraphicsRectItem* rectItem = scene->addRect(QRectF(0, 0, patchRect.width(), patchRect.height()), blackPen, grayBrush);
      rectItem->setPos(patchRect.x(), patchRect.y());

      QString text = boxObject["text"].toString().simplified();
      if ("inlet" == className)
      {
         const int index = boxObject["index"].toInt();
         text = "IN\n" + QString::number(index);

         const QString toolTip = boxObject["comment"].toString();
         rectItem->setToolTip(toolTip);
      }
      else if ("outlet" == className)
      {
         const int index = boxObject["index"].toInt();
         text = "OUT\n" + QString::number(index);

         const QString toolTip = boxObject["comment"].toString();
         rectItem->setToolTip(toolTip);
      }
      if (text.isEmpty())
         text = className;

      QGraphicsItem* foregroundItem = nullptr;

      QGraphicsSimpleTextItem* textItem = scene->addSimpleText(text, font);
      textItem->setPos(patchRect.x() + 5, patchRect.y() + 5);
      foregroundItem = textItem;

      const int inletCount = boxObject["numinlets"].toInt();
      const int outletCount = boxObject["numoutlets"].toInt();

      const QString id = boxObject["id"].toString();
      idMap[id] = {rectItem, foregroundItem, inletCount, outletCount};
   }

   return idMap;
}

void Schema::Widget::makeLines(const QJsonObject patcherObject, const IdMap& idMap)
{
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();
      if (!idMap.contains(sourceId))
         continue;

      const Box sourceBox = idMap[sourceId];
      const int sourceOffset = sourceArray.at(1).toInt();
      const QRectF sourceRect = QRectF(sourceBox.rectItem->pos().x(), sourceBox.rectItem->pos().y(), sourceBox.rectItem->rect().width(), sourceBox.rectItem->rect().height());

      const int outletDist = sourceRect.width() / sourceBox.outletCount;
      const int sourceX = 10 + sourceRect.x() + (outletDist * sourceOffset);
      const int sourceY = sourceRect.y() + sourceRect.height();

      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();
      if (!idMap.contains(destId))
         continue;

      const Box destBox = idMap[destId];
      const int destOffset = destArray.at(1).toInt();
      const QRectF destRect = QRectF(destBox.rectItem->pos().x(), destBox.rectItem->pos().y(), destBox.rectItem->rect().width(), destBox.rectItem->rect().height());

      const int inletDist = destRect.width() / destBox.inletCount;
      const int destX = 10 + destRect.x() + (inletDist * destOffset);
      const int destY = destRect.y();

      scene->addLine(sourceX, sourceY, destX, destY, blackPen);
   }
}

void Schema::Widget::moveItems(const IdMap& idMap)
{
   auto compileMinPoint = [&]()
   {
      QPointF minPoint;
      bool first = true;

      for (Box box : idMap.values())
      {
         const QPointF ipos = box.rectItem->pos();
         if (first)
         {
            minPoint = ipos;
            first = false;
            continue;
         }

         if (minPoint.x() > ipos.x())
            minPoint.setX(ipos.x());

         if (minPoint.y() > ipos.y())
            minPoint.setY(ipos.y());
      }
      return minPoint;
   };

   const QPointF minPoint = compileMinPoint();
   for (Box box : idMap.values())
   {
      const QPointF oldPos = box.rectItem->pos();
      QPointF newPos = oldPos - minPoint;
      box.rectItem->setPos(newPos);
      if (box.foregroundItem)
         box.foregroundItem->setPos(newPos + QPointF(5, 5));
   }
}
