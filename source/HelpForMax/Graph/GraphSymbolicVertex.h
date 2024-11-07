#ifndef GraphSymbolicVertexH
#define GraphSymbolicVertexH

#include <QList>

namespace Graph
{
   namespace Symbolic
   {
      class Vertex
      {
      public:
         using PrtList = QList<Vertex*>;
         using ConstPtrList = QList<const Vertex*>;

      public:
         Vertex(const QString& name);

      public:
         const QString& getName() const;

      private:
         QString name;
      };
   } // namespace Symbolic
} // namespace Graph

#endif // NOT GraphSymbolicVertexH
