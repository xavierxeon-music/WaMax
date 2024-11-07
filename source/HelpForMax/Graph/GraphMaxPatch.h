#ifndef GraphMaxPatchH
#define GraphMaxPatchH

#include "GraphSymbolicGraph.h"

#include "GraphMaxObject.h"

namespace Graph
{
   namespace Max
   {

      class Patch : public Symbolic::Graph
      {
      public:
         Patch();

      public:
         void read(const QString& patchFileName);
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
