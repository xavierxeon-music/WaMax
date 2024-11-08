#include "GraphAbstractEdge.h"

#include <cmath>

const double Graph::Abstract::Edge::invalidWeight = std::numeric_limits<double>::quiet_NaN();

Graph::Abstract::Edge::Edge(Vertex* vertexA, Vertex* vertexB, const double& weightForward, const double& weightBackward)
   : vertexA(vertexA)
   , vertexB(vertexB)
   , weightForward(weightForward)
   , weightBackward(weightBackward)
{
}

Graph::Abstract::Edge::~Edge()
{
}

double Graph::Abstract::Edge::getForwardWeight() const
{
   return weightForward;
}

double Graph::Abstract::Edge::getBackwardWeight() const
{
   return weightBackward;
}

bool Graph::Abstract::Edge::hasForwardLink() const
{
   return !std::isnan(getForwardWeight());
}

bool Graph::Abstract::Edge::hasBackwardLink() const
{
   return !std::isnan(getBackwardWeight());
}

bool Graph::Abstract::Edge::linksVertex(const Vertex* vertex) const
{
   if (vertex == vertexA)
      return true;
   else if (vertex == vertexB)
      return true;

   return false;
}

const Graph::Abstract::Vertex* Graph::Abstract::Edge::getVertexA() const
{
   return vertexA;
}

const Graph::Abstract::Vertex* Graph::Abstract::Edge::getVertexB() const
{
   return vertexB;
}
