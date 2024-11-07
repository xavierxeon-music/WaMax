#ifndef SDULibGraphEdgeAbstractH
#define SDULibGraphEdgeAbstractH

#include "SDULibGraphExportDef.h"

#include <QList>

namespace SDULibGraph
{
   class Vertex;
   class Graph;

   namespace Edge
   {
      class SDULIBGRAPH_DECLSPEC Abstract
      {
      public:
         using PtrList = QList<Abstract*>;
         using ConstPtrList = QList<const Abstract*>;
         static const double invalidWeight;

      public:
         Abstract(Vertex* vertexA, Vertex* vertexB);
         virtual ~Abstract();

      public:
         virtual double getForwardWeight() const = 0;
         virtual double getBackwardWeight() const = 0;
         bool hasForwardLink() const;
         bool hasBackwardLink() const;
         bool linksVertex(const Vertex* vertex) const;

         const Vertex* getVertexA() const;
         const Vertex* getVertexB() const;

      protected:
         Vertex* vertexA;
         Vertex* vertexB;

      private:
         friend class Graph;
      };
   } // namespace Edge
} // namespace SDULibGraph

#endif // NOT SDULibGraphEdgeAbstractH
