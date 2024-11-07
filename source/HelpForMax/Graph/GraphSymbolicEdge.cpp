#include "GraphSymbolicEdge.h"

#include <cmath>

const double Graph::Symbolic::Edge::invalidWeight = std::numeric_limits<double>::quiet_NaN();

Graph::Symbolic::Edge::Edge(Vertex* vertexA, Vertex* vertexB, const double& weightForward, const double& weightBackward)
   : vertexA(vertexA)
   , vertexB(vertexB)
   , weightForward(weightForward)
   , weightBackward(weightBackward)
{
}

Graph::Symbolic::Edge::~Edge()
{
}

double Graph::Symbolic::Edge::getForwardWeight() const
{
   return weightForward;
}

double Graph::Symbolic::Edge::getBackwardWeight() const
{
   return weightBackward;
}

bool Graph::Symbolic::Edge::hasForwardLink() const
{
   return !std::isnan(getForwardWeight());
}

bool Graph::Symbolic::Edge::hasBackwardLink() const
{
   return !std::isnan(getBackwardWeight());
}

bool Graph::Symbolic::Edge::linksVertex(const Vertex* vertex) const
{
   if (vertex == vertexA)
      return true;
   else if (vertex == vertexB)
      return true;

   return false;
}

const Graph::Symbolic::Vertex* Graph::Symbolic::Edge::getVertexA() const
{
   return vertexA;
}

const Graph::Symbolic::Vertex* Graph::Symbolic::Edge::getVertexB() const
{
   return vertexB;
}
