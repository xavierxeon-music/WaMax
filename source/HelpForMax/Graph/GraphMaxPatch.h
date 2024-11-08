#ifndef GraphMaxPatchH
#define GraphMaxPatchH

#include "GraphSymbolicGraph.h"

#include "GraphMaxLine.h"
#include "GraphMaxObject.h"

namespace Graph
{
   namespace Max
   {
      class Patch : public Symbolic::Graph<Object, Line>
      {
      public:
         Patch();

      public:
         void read(const QString& patchFileName);
         Object::List findAll(const Object::Type& type) const;
         Object::List findAll(const QList<Object::Type>& typeList) const;
         void analyse();

      private:
         void readObjects(const QJsonObject patcherObject);
         void readLines(const QJsonObject patcherObject);

      private:
         Object::IdMap idMap;
      };
   } // namespace Max
} // namespace Graph

#endif // NOT GraphMaxPatchH
