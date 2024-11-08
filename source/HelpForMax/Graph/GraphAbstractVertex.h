#ifndef GraphAbstractVertexH
#define GraphAbstractVertexH

#include <QList>

namespace Graph
{
   namespace Abstract
   {
      class Vertex
      {
      public:
         using PrtList = QList<Vertex*>;
         using ConstPtrList = QList<const Vertex*>;

      public:
         Vertex(const QString& name = QString());

      public:
         const QString& getName() const;

      protected:
         QString name;
      };
   } // namespace Abstract
} // namespace Graph

#endif // NOT GraphAbstractVertexH
