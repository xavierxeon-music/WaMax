#include "GraphSymbolicAlgorithm.h"

#include <QDebug>
#include <QMap>
#include <QQueue>

#include "GraphSymbolicGraph.h"

// path

const double Graph::Symbolic::Algorithm::Path::invalidDistance = std::numeric_limits<double>::max();

// tree

Graph::Symbolic::Algorithm::Tree::VertexData Graph::Symbolic::Algorithm::Tree::findData(const int vertexIndex) const
{
   for (const VertexData& data : verticies)
   {
      if (data.vertexIndex == vertexIndex)
         return data;
   }

   return VertexData{};
}

Graph::Symbolic::Algorithm::Path Graph::Symbolic::Algorithm::Tree::compilePath(const int vertexIndex) const
{
   Path path;

   int searchIndex = vertexIndex;
   while (-1 != searchIndex)
   {
      path.distance += 1.0;
      path.verticies.append(searchIndex);

      VertexData data = findData(searchIndex);

      searchIndex = data.parentVertexIndex;
   }

   return path;
}

int Graph::Symbolic::Algorithm::Tree::compileDepth(const int vertexIndex) const
{
   return findData(vertexIndex).depth;
}

// algorithm

Graph::Symbolic::Algorithm::Algorithm(const Graph* graph)
   : graph(graph)
   , edgeMatrix()
{
   const EdgeData::Row defaultEntries = EdgeData::Row(graph->vertexCount(), EdgeData{});
   edgeMatrix = EdgeData::Matrix(graph->vertexCount(), defaultEntries);

   for (int edgeIndex = 0; edgeIndex < graph->edgeCount(); edgeIndex++)
   {
      const Edge* edge = graph->getEdge(edgeIndex);

      const int from = graph->vertexIndex(edge->getVertexA());
      const int to = graph->vertexIndex(edge->getVertexB());

      if (edge->hasForwardLink())
         edgeMatrix[from][to] = {edge->getForwardWeight(), edgeIndex, true};

      if (edge->hasBackwardLink())
         edgeMatrix[to][from] = {edge->getBackwardWeight(), edgeIndex, false};
   }
}

Graph::Symbolic::Algorithm::Tree Graph::Symbolic::Algorithm::depthFirst(const Vertex* vertexStart) const
{
   const int startIndex = graph->vertexIndex(vertexStart);
   QVector<bool> visited(graph->vertexCount(), false);

   Tree tree;

   std::function<void(int, int, int)> traverse = [&](int vertIndex, int parentIndex, int depth)
   {
      tree.verticies.append(Tree::VertexData{vertIndex, parentIndex, depth});
      visited[vertIndex] = true;

      const EdgeData::Row edges = edgeMatrix.at(vertIndex);
      for (int toIndex = 0; toIndex < graph->vertexCount(); toIndex++)
      {
         if (-1 == edges[toIndex].index)
            continue;

         if (visited.at(toIndex))
            continue;

         traverse(toIndex, vertIndex, depth + 1);
      }
   };

   traverse(startIndex, -1, 0);

   return tree;
}

Graph::Symbolic::Algorithm::Tree Graph::Symbolic::Algorithm::breadthFirst(const Vertex* vertexStart) const
{
   const int startIndex = graph->vertexIndex(vertexStart);
   QVector<bool> visited(graph->vertexCount(), false);

   struct Param
   {
      int index;
      int depth;
   };

   QQueue<Param> queue;

   Tree tree;

   visited[startIndex] = true;
   queue.enqueue(Param{startIndex, 0});
   tree.verticies.append(Tree::VertexData{startIndex, -1, 0});

   while (!queue.isEmpty())
   {
      const Param current = queue.dequeue();
      const int nextDepth = current.depth + 1;

      for (const int toIndex : compileAdjacencyList(current.index))
      {
         if (visited.at(toIndex))
            continue;

         visited[toIndex] = true;
         queue.enqueue(Param{toIndex, nextDepth});
         tree.verticies.append(Tree::VertexData{toIndex, current.index, nextDepth});
      }
   }

   return tree;
}

Graph::Symbolic::Algorithm::Path::Map Graph::Symbolic::Algorithm::pathDijkstra(const Vertex* vertexStart) const
{
   QVector<double> distances(graph->vertexCount(), Path::invalidDistance);
   const int startIndex = graph->vertexIndex(vertexStart);
   distances[startIndex] = 0;

   QVector<int> predecessors(graph->vertexCount(), -1);
   QVector<bool> visited(graph->vertexCount(), false);

   auto findShortestUnvisited = [&]()
   {
      double min_distance = Path::invalidDistance;
      int fromIndex = -1;

      for (int index = 0; index < graph->vertexCount(); index++)
      {
         if (!visited[index] && distances[index] < min_distance)
         {
            min_distance = distances[index];
            fromIndex = index;
         }
      }

      return fromIndex;
   };

   for (int loop = 0; loop < graph->vertexCount(); loop++)
   {
      int fromIndex = findShortestUnvisited();
      if (-1 == fromIndex)
         break;

      visited[fromIndex] = true;

      for (const int toIndex : compileAdjacencyList(fromIndex))
      {
         if (visited[toIndex])
            continue;

         const double toDistance = distances[fromIndex] + edgeMatrix[fromIndex][toIndex].weight;
         if (toDistance < distances[toIndex])
         {
            distances[toIndex] = toDistance;
            predecessors[toIndex] = fromIndex;
         }
      }
   }

   Path::Map pathMap;
   for (int index = 0; index < graph->vertexCount(); index++)
   {
      QList<int> verticies;
      int current = index;
      while (current != startIndex && -1 != current)
      {
         verticies.prepend(current);
         current = predecessors[current];
      }
      verticies.prepend(startIndex);

      pathMap.insert(index, {distances[index], verticies});
   }
   return pathMap;
}

Graph::Symbolic::Algorithm::TreeEdges Graph::Symbolic::Algorithm::compileTreeEdges(const Tree& tree) const
{
   TreeEdges treeEdges;

   for (const Tree::VertexData& data : tree.verticies)
   {
      const int edgeIndex = findEdgeIndex(data.parentVertexIndex, data.vertexIndex);
      if (-1 == edgeIndex)
         continue;

      treeEdges.treeEdges.append(edgeIndex);
   }

   return treeEdges;
}

Graph::Symbolic::Algorithm::IndexList Graph::Symbolic::Algorithm::topologicalSort() const
{
   QVector<int> in_degree(graph->vertexCount(), 0);
   QQueue<int> startNodes;

   {
      for (int fromIndex = 0; fromIndex < graph->vertexCount(); fromIndex++)
      {
         for (const int toIndex : compileAdjacencyList(fromIndex))
         {
            in_degree[toIndex]++;
         }
      }

      for (int index = 0; index < graph->vertexCount(); index++)
      {
         if (0 == in_degree[index])
            startNodes.enqueue(index);
      }
   }

   IndexList list;
   int visitedNodeCount = 0;

   while (!startNodes.isEmpty())
   {
      const int fromIndex = startNodes.dequeue();
      list.append(fromIndex);

      for (const int toIndex : compileAdjacencyList(fromIndex))
      {
         --in_degree[toIndex];
         if (0 == in_degree[toIndex])
            startNodes.enqueue(toIndex);
      }

      visitedNodeCount++;
   }

   qDebug() << visitedNodeCount << graph->vertexCount();

   return list;
}

int Graph::Symbolic::Algorithm::findEdgeIndex(const int vertexIndexA, const int vertexIndexB) const
{
   if (-1 == vertexIndexA || -1 == vertexIndexB)
      return -1;

   const Vertex* vertexA = graph->getVertex(vertexIndexA);
   const Vertex* vertexB = graph->getVertex(vertexIndexB);

   return graph->findEdgeIndex(vertexA, vertexB);
}

Graph::Symbolic::Algorithm::IndexList Graph::Symbolic::Algorithm::compileAdjacencyList(const int vertexIndex) const
{
   IndexList indexList;

   const EdgeData::Row& toList = edgeMatrix.at(vertexIndex);
   for (int toIndex = 0; toIndex < graph->vertexCount(); toIndex++)
   {
      if (-1 == toList.at(toIndex).index)
         continue;

      indexList.append(toIndex);
   }

   return indexList;
}
