#ifndef GraphAbstractGraphH
#define GraphAbstractGraphH

#include "GraphAbstractEdge.h"
#include "GraphAbstractVertex.h"

namespace Graph
{
   namespace Abstract
   {
      class Graph
      {
      public:
         Graph();

      public:
         int addVertex(Vertex* vertex);
         bool removeVertex(Vertex* vertex, bool deleteVertex = false);
         int vertexIndex(const Vertex* vertex) const;
         int vertexCount() const;
         Vertex* getVertex(int vertexIndex) const;

         int addEdge(Edge* edge);
         bool removeEdge(Edge* edge, bool deleteEdge = false);
         int edgeIndex(const Edge* edge) const;
         int edgeCount() const;
         Edge* getEdge(int edgeIndex) const;
         int findEdgeIndex(const Vertex* vertexA, const Vertex* vertexB) const;

         void clear(bool deleteAll = false);

      private:
         Vertex::PrtList vertexList;
         Edge::PtrList edgeList;
      };

   } // namespace Abstract

} // namespace Graph

#endif // NOT GraphAbstractGraphH
