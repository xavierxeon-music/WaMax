#include "MaxPatcher.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "DiscreteMathsAlgorithm.h"

Max::Patcher::Patcher()
   : Ref::Structure()
   , DiscreteMaths::Graph()
   , typeBuffer()
{
}

void Max::Patcher::readPatch(const QString& patchFileName)
{
   typeBuffer.clear();
   DiscreteMaths::Graph::clear(true);

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

   const Object::IdMap idMap = readObjects(patcherObject);
   readLines(patcherObject, idMap);

   analyse();

   Ref::Structure::clear();
   buildStructureArguments();
   buildStructureTypedMessages();
   buildStructureNamedMessages();
   buildStructureOutputs();
}

Max::Object* Max::Patcher::getVertexCast(int vertexIndex) const
{
   Object* vertex = static_cast<Object*>(getVertex(vertexIndex));
   return vertex;
}

Max::Line* Max::Patcher::getEdgeCast(int edgeIndex) const
{
   Line* edge = static_cast<Line*>(getEdge(edgeIndex));
   return edge;
}

void Max::Patcher::analyse()
{
   static const QList<Object::Type> sourceTypeList = {Object::Type::PatcherArgs, Object::Type::Inlet};
   static const QList<Object::Type> processTypeList = {Object::Type::Route, Object::Type::RoutePass, Object::Type::TypeRoute, Object::Type::Unpack};

   const Object::List sources = findAll(sourceTypeList, false);
   const Object::List processors = findAll(processTypeList, false);

   DiscreteMaths::Algorithm algo(this);
   for (Object* source : sources)
   {
      const DiscreteMaths::Algorithm::Tree tree = algo.breadthFirst(source);
      for (Object* processor : processors)
      {
         const int targetIndex = vertexIndex(processor);
         const DiscreteMaths::Algorithm::Path path = tree.compilePath(targetIndex);
         const int depth = path.verticies.count();

         auto pathIsValid = [&]()
         {
            if (0 == depth)
               return false;

            for (int index = 0; index < depth; index++)
            {
               const int vertexIndexA = path.verticies.at(index);
               Object* outletObject = getVertexCast(vertexIndexA);

               if (!sourceTypeList.contains(outletObject->type) && !processTypeList.contains(outletObject->type))
                  return false;
            }
            return true;
         };

         if (!pathIsValid())
            continue;

         for (int index = 1; index < depth; index++)
         {
            const int vertexIndexA = path.verticies.at(index);
            Object* outletObject = getVertexCast(vertexIndexA);
            outletObject->isParamObject = true;

            const int vertexIndexB = path.verticies.at(index - 1);
            Object* inletObject = getVertexCast(vertexIndexB);
            inletObject->isParamObject = true;

            const int edgeIndex = findEdgeIndex(outletObject, inletObject);
            Line* line = getEdgeCast(edgeIndex);
            line->isParamLine = true;
         }
      }
   }
}

void Max::Patcher::buildStructureArguments()
{
   const Object::List patcherArgs = findAll(Object::Type::PatcherArgs, true);
   const Object::List unpackArgs = findAll(Object::Type::Unpack, true);

   for (const Object* object : patcherArgs)
   {
      const Object* unpackObject = nullptr;
      for (const Object* object2 : unpackArgs)
      {
         if (isChildOf(object2, object))
         {
            unpackObject = object2;
            break;
         }
      }

      const int argumentIndex = object->text.indexOf("@");
      const QString text = object->text.mid(0, argumentIndex);
      const QStringList argumentNameList = text.split(" ", Qt::SkipEmptyParts);

      const QStringList unpackTypeList = unpackObject ? unpackObject->text.split(" ", Qt::SkipEmptyParts) : QStringList();

      for (int index = 1; index < argumentNameList.count(); index++)
      {
         Ref::Structure::Argument argument;
         argument.name = argumentNameList.at(index);

         if (unpackTypeList.size() > index)
         {
            const QString type = unpackTypeList.at(index);
            if ("s" == type)
               argument.dataType = Max::DataType::Symbol;
            else if ("f" == type)
               argument.dataType = Max::DataType::Float;
            else if ("i" == type)
               argument.dataType = Max::DataType::Integer;
            else if ("b" == type)
               argument.dataType = Max::DataType::Bang;
            else if ("l" == type)
               argument.dataType = Max::DataType::List;
            else
               argument.dataType = Max::DataType::Anything;
         }

         argumentList.append(argument);
      }
   }
}

void Max::Patcher::buildStructureTypedMessages()
{
}

void Max::Patcher::buildStructureNamedMessages()
{
}

void Max::Patcher::buildStructureOutputs()
{
}

Max::Object::List Max::Patcher::findAll(const Object::Type& type, bool paramObjectsOnly) const
{
   return findAll(QList<Object::Type>() << type, paramObjectsOnly);
}

Max::Object::List Max::Patcher::findAll(const QList<Object::Type>& typeList, bool paramObjectsOnly) const
{
   Object::List list;
   for (const Object::Type& type : typeList)
   {
      if (!typeBuffer.contains(type))
         continue;

      if (paramObjectsOnly)
      {
         for (Object* object : typeBuffer.value(type))
         {
            if (object->isParamObject)
               list.append(object);
         }
      }
      else
      {
         list.append(typeBuffer.value(type));
      }
   }

   return list;
}

bool Max::Patcher::isChildOf(const Object* object, const Object* parent) const
{
   for (int edgeIndex = 0; edgeIndex < edgeCount(); edgeIndex++)
   {
      Line* line = getEdgeCast(edgeIndex);
      if (line->getVertexA() != parent) // outlet
         continue;

      if (line->getVertexB() != object) // inlet
         continue;

      return true;
   }
   return false;
}

Max::Object::IdMap Max::Patcher::readObjects(const QJsonObject patcherObject)
{
   static const QStringList skipList = {"comment", "panel"};

   Object::IdMap idMap;
   const QJsonArray boxArray = patcherObject["boxes"].toArray();

   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if (skipList.contains(className))
         continue;

      Object* object = new Object(boxObject);
      addVertex(object);

      Object::List& typeList = typeBuffer[object->type];
      if (!typeList.contains(object))
         typeList.append(object);

      idMap[object->id] = object;
   }

   return idMap;
}

void Max::Patcher::readLines(const QJsonObject patcherObject, const Object::IdMap& idMap)
{
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      Line* line = new Line(lineObject, idMap);
      addEdge(line);
   }
}
