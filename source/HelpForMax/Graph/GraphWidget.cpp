#include "GraphWidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QGraphicsItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMouseEvent>
#include <QSettings>
#include <QWheelEvent>

Graph::Widget::Widget(QWidget* parent)
   : QGraphicsView(parent)
   , scene(nullptr)
   , patchFileName()
   , blackPen(Qt::black)
   , whiteBrush(Qt::white)
   , blackBrush(Qt::black)
   , font()
   , zoomLevel(1.0)
{
   font.setPixelSize(10);

   QBrush bgBrush = QApplication::palette().window();

   scene = new QGraphicsScene(this);
   scene->setBackgroundBrush(bgBrush);

   setScene(scene);
   setAlignment(Qt::AlignLeft | Qt::AlignTop);
   setViewportMargins(10, 10, 0, 0);

   {
      QSettings settings;
      zoomLevel = settings.value("Graph/Zoom", 1.0).toDouble();
   }
   updateZoom(false);
}

void Graph::Widget::slotLoad(const QString& patchFileName)
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
   makeLines(patcherObject, idMap);

   updateZoom(false);
}

Graph::Widget::IdMap Graph::Widget::makeObjects(const QJsonObject patcherObject)
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

      Box box;

      box.rectItem = scene->addRect(QRectF(0, 0, patchRect.width(), patchRect.height()), blackPen, whiteBrush);
      box.rectItem->setPos(patchRect.x(), patchRect.y());

      QString text = boxObject["text"].toString().simplified();
      if ("inlet" == className)
      {
         const int index = boxObject["index"].toInt();
         text = "IN\n" + QString::number(index);

         const QString toolTip = boxObject["comment"].toString();
         box.rectItem->setToolTip(toolTip);
      }
      else if ("outlet" == className)
      {
         const int index = boxObject["index"].toInt();
         text = "OUT\n" + QString::number(index);

         const QString toolTip = boxObject["comment"].toString();
         box.rectItem->setToolTip(toolTip);
      }
      if (text.isEmpty())
         text = className;

      QGraphicsSimpleTextItem* textItem = scene->addSimpleText(text, font);
      textItem->setPos(patchRect.x() + 5, patchRect.y() + 5);
      box.foregroundItems.append(textItem);

      QGraphicsRectItem* topBar = scene->addRect(QRectF(0, 0, patchRect.width(), 2), blackPen, blackBrush);
      topBar->setPos(patchRect.x(), patchRect.y());
      box.foregroundItems.append(topBar);

      QGraphicsRectItem* bottomBar = scene->addRect(QRectF(0, patchRect.height(), patchRect.width(), 2), blackPen, blackBrush);
      bottomBar->setPos(patchRect.x(), patchRect.y());
      box.foregroundItems.append(bottomBar);

      box.inletCount = boxObject["numinlets"].toInt();
      box.outletCount = boxObject["numoutlets"].toInt();

      const QString id = boxObject["id"].toString();
      idMap[id] = box;
   }

   return idMap;
}

void Graph::Widget::makeLines(const QJsonObject patcherObject, const IdMap& idMap)
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

void Graph::Widget::wheelEvent(QWheelEvent* wheelEvent)
{
   const double delta = 0.001 * wheelEvent->angleDelta().y();
   const double factor = std::pow(2.0, delta);

   zoomLevel *= factor;
   updateZoom(true);
}

void Graph::Widget::mouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
   QGraphicsItem* item = itemAt(mouseEvent->pos());
   if (!item)
   {
      zoomLevel = 1.0;
      updateZoom(true);
   }
}

void Graph::Widget::keyPressEvent(QKeyEvent* event)
{
   const bool enabled = Qt::ShiftModifier & event->modifiers();
   setDragMode(enabled ? ScrollHandDrag : NoDrag);
}

void Graph::Widget::keyReleaseEvent(QKeyEvent* event)
{
   const bool enabled = Qt::ShiftModifier & event->modifiers();
   setDragMode(enabled ? ScrollHandDrag : NoDrag);
}

void Graph::Widget::updateZoom(bool save)
{
   setTransform(QTransform::fromScale(zoomLevel, zoomLevel));

   if (!save)
      return;

   QSettings settings;
   settings.setValue("Graph/Zoom", zoomLevel);
}
