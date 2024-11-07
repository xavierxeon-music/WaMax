#include "../SDULibGraphGraph.h"

SDULibGraph::Graph::Graph()
   : vertexList()
   , edgeList()
{
}

int SDULibGraph::Graph::addVertex(Vertex* vertex)
{
   if (vertexList.contains(vertex))
      return -1;

   vertexList.append(vertex);
   return vertexList.count() - 1;
}

bool SDULibGraph::Graph::removeVertex(Vertex* vertex, bool deleteVertex)
{
   if (!vertexList.contains(vertex))
      return false;

   vertexList.removeAll(vertex);

   Edge::Abstract::PtrList edgeRemoveList; // do not remove element from list while iterating
   for (Edge::Abstract* edge : edgeList)
   {
      if (edge->linksVertex(vertex))
         edgeRemoveList.append(edge);
   }
   for (Edge::Abstract* edge : edgeRemoveList)
   {
      edgeList.removeAll(edge);
   }

   if (deleteVertex)
      delete vertex;

   return true;
}

int SDULibGraph::Graph::vertexIndex(const Vertex* constVertex) const
{
   Vertex* vertex = const_cast<Vertex*>(constVertex);

   if (!vertexList.contains(vertex))
      return -1;

   return vertexList.indexOf(vertex);
}

int SDULibGraph::Graph::vertexCount() const
{
   return vertexList.count();
}

SDULibGraph::Vertex* SDULibGraph::Graph::getVertex(int vertexIndex) const
{
   if (vertexIndex < 0 || vertexIndex >= vertexList.count())
      return nullptr;

   return vertexList.at(vertexIndex);
}

int SDULibGraph::Graph::addEdge(Edge::Abstract* edge)
{
   if (edgeList.contains(edge))
      return -1;

   edgeList.append(edge);
   return edgeList.count() - 1;
}

bool SDULibGraph::Graph::removeEdge(Edge::Abstract* edge, bool deleteEdge)
{
   if (!edgeList.contains(edge))
      return false;

   edgeList.removeAll(edge);

   if (deleteEdge)
      delete edge;

   return true;
}

int SDULibGraph::Graph::edgeIndex(const Edge::Abstract* constEdge) const
{
   Edge::Abstract* edge = const_cast<Edge::Abstract*>(constEdge);

   if (!edgeList.contains(edge))
      return -1;

   return edgeList.indexOf(edge);
}

int SDULibGraph::Graph::edgeCount() const
{
   return edgeList.count();
}

SDULibGraph::Edge::Abstract* SDULibGraph::Graph::getEdge(int edgeIndex) const
{
   if (edgeIndex < 0 || edgeIndex >= edgeList.count())
      return nullptr;

   return edgeList.at(edgeIndex);
}

int SDULibGraph::Graph::findEdgeIndex(const Vertex* vertexA, const Vertex* vertexB) const
{
   for (int edgeIndex = 0; edgeIndex < edgeList.count(); edgeIndex++)
   {
      const Edge::Abstract* edge = edgeList.at(edgeIndex);
      if (edge->getVertexA() == vertexA && edge->getVertexB() == vertexB)
         return edgeIndex;
   }

   return -1;
}

void SDULibGraph::Graph::clear(bool deleteAll)
{
   if (deleteAll)
   {
      for (const Vertex* vertex : vertexList)
         delete vertex;

      for (const Edge::Abstract* edge : edgeList)
         delete edge;
   }

   vertexList.clear();
   edgeList.clear();
}
