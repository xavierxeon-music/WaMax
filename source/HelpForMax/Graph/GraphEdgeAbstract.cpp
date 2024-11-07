#include "../SDULibGraphEdgeAbstract.h"

#include <cmath>

const double SDULibGraph::Edge::Abstract::invalidWeight = std::numeric_limits<double>::quiet_NaN();

SDULibGraph::Edge::Abstract::Abstract(Vertex* vertexA, Vertex* vertexB)
   : vertexA(vertexA)
   , vertexB(vertexB)
{
}

SDULibGraph::Edge::Abstract::~Abstract()
{
}

bool SDULibGraph::Edge::Abstract::hasForwardLink() const
{
   return !std::isnan(getForwardWeight());
}

bool SDULibGraph::Edge::Abstract::hasBackwardLink() const
{
   return !std::isnan(getBackwardWeight());
}

bool SDULibGraph::Edge::Abstract::linksVertex(const Vertex* vertex) const
{
   if (vertex == vertexA)
      return true;
   else if (vertex == vertexB)
      return true;

   return false;
}

const SDULibGraph::Vertex* SDULibGraph::Edge::Abstract::getVertexA() const
{
   return vertexA;
}

const SDULibGraph::Vertex* SDULibGraph::Edge::Abstract::getVertexB() const
{
   return vertexB;
}
