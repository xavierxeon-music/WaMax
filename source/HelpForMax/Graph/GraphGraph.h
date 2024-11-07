#ifndef SDULibGraphGraphH
#define SDULibGraphGraphH

#include "SDULibGraphExportDef.h"

#include "SDULibGraphEdgeAbstract.h"
#include "SDULibGraphVertex.h"

namespace SDULibGraph
{
   class SDULIBGRAPH_DECLSPEC Graph
   {
   public:
      Graph();

   public:
      int addVertex(Vertex* vertex);
      bool removeVertex(Vertex* vertex, bool deleteVertex = false);
      int vertexIndex(const Vertex* vertex) const;
      int vertexCount() const;
      Vertex* getVertex(int vertexIndex) const;

      int addEdge(Edge::Abstract* edge);
      bool removeEdge(Edge::Abstract* edge, bool deleteEdge = false);
      int edgeIndex(const Edge::Abstract* edge) const;
      int edgeCount() const;
      Edge::Abstract* getEdge(int edgeIndex) const;
      int findEdgeIndex(const Vertex* vertexA, const Vertex* vertexB) const;

      void clear(bool deleteAll = false);

   private:
      Vertex::PrtList vertexList;
      Edge::Abstract::PtrList edgeList;
   };
} // namespace SDULibGraph

#endif // NOT SDULibGraphGraphH
