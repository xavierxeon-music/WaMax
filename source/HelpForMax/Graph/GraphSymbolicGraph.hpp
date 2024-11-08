#ifndef GraphSymbolicGraphHPP
#define GraphSymbolicGraphHPP

#include "GraphSymbolicGraph.h"

template <typename VertexType, typename EdgeType>
Graph::Symbolic::Graph<VertexType, EdgeType>::Graph()
   : Abstract::Graph()
{
}

template <typename VertexType, typename EdgeType>
VertexType* Graph::Symbolic::Graph<VertexType, EdgeType>::getVertexCast(int vertexIndex) const
{
   VertexType* vertex = static_cast<VertexType*>(getVertex(vertexIndex));
   return vertex;
}

template <typename VertexType, typename EdgeType>
EdgeType* Graph::Symbolic::Graph<VertexType, EdgeType>::getEdgeCast(int edgeIndex) const
{
   EdgeType* edge = static_cast<EdgeType*>(getEdge(edgeIndex));
   return edge;
}

#endif // NOT GraphSymbolicGraphHPP
