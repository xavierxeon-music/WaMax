#ifndef GraphSymbolicGraphH
#define GraphSymbolicGraphH

#include "GraphAbstractGraph.h"

namespace Graph
{
   namespace Symbolic
   {
      template <typename VertexType, typename EdgeType>
      class Graph : public Abstract::Graph
      {
      public:
         Graph();

      public:
         VertexType* getVertexCast(int vertexIndex) const;
         EdgeType* getEdgeCast(int edgeIndex) const;
      };
   } // namespace Symbolic

} // namespace Graph

#ifndef GraphSymbolicGraphHPP
#include "GraphSymbolicGraph.hpp"
#endif // NOT GraphSymbolicGraphHPP

#endif // NOT GraphSymbolicGraphH
