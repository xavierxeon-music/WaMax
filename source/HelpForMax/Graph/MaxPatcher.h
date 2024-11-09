#ifndef MaxPatcherH
#define MaxPatcherH

#include "StructureGraph.h"

#include "MaxLine.h"
#include "MaxObject.h"

namespace Max
{
   class Patcher : public Structure::Graph
   {
   public:
      Patcher();

   public:
      void readPatch(const QString& patchFileName);
      Object::List findAll(const Object::Type& type) const;
      Object::List findAll(const QList<Object::Type>& typeList) const;
      void analyse();
      Object* getVertexCast(int vertexIndex) const;
      Line* getEdgeCast(int edgeIndex) const;

   private:
      using TypeBuffer = QMap<Object::Type, Object::List>;

   private:
      Object::IdMap readObjects(const QJsonObject patcherObject);
      void readLines(const QJsonObject patcherObject, const Object::IdMap& idMap);

   private:
      TypeBuffer typeBuffer;
   };
} // namespace Max

#endif // NOT MaxPatcherH
