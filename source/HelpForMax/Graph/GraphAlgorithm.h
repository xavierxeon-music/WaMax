#ifndef SDULibGraphAlgorithmH
#define SDULibGraphAlgorithmH

#include "SDULibGraphExportDef.h"

#include "SDULibGraphEdgeAbstract.h"
#include "SDULibGraphVertex.h"

namespace SDULibGraph
{
   class Graph;

   class SDULIBGRAPH_DECLSPEC Algorithm
   {
   public:
      using IndexList = QList<int>;

      struct SDULIBGRAPH_DECLSPEC Path
      {
         static const double invalidDistance;

         double distance = invalidDistance;
         IndexList verticies;

         using Map = QMap<int, Path>; // end vertex index vs path
      };

      struct SDULIBGRAPH_DECLSPEC Tree
      {
         struct SDULIBGRAPH_DECLSPEC VertexData
         {
            int vertexIndex = -1;
            int parentVertexIndex = -1;
            int depth = 0;

            using List = QList<VertexData>;
         };

         VertexData::List verticies; // visited in order

         Path compilePath(const int vertexIndex) const;
         int compileDepth(const int vertexIndex) const;

         VertexData findData(const int vertexIndex) const;
      };

      struct SDULIBGRAPH_DECLSPEC TreeEdges
      {
         IndexList treeEdges;    // edges used for visit
         IndexList forwardEdges; // unvisited, in order
         IndexList backEdges;    // unvisted, against order
      };

   public:
      Algorithm(const Graph* graph);

   public:
      Tree depthFirst(const Vertex* vertexStart) const;
      Tree breadthFirst(const Vertex* vertexStart) const;

      Path::Map pathDijkstra(const Vertex* vertexStart) const;
      TreeEdges compileTreeEdges(const Tree& tree) const;

      IndexList topologicalSort() const;

   private:
      struct EdgeData
      {
         double weight = Edge::Abstract::invalidWeight;
         int index = -1;
         bool forward = true;

         using Row = QVector<EdgeData>;
         using Matrix = QVector<Row>;
      };

   private:
      int findEdgeIndex(const int vertexIndexA, const int vertexIndexB) const;

      IndexList compileAdjacencyList(const int vertexIndex) const;

   private:
      const Graph* graph;
      EdgeData::Matrix edgeMatrix;
   };
} // namespace SDULibGraph

#endif // NOT SDULibGraphAlgorithmH
