#include "StructureEdge.h"

#include <cmath>

const double Structure::Edge::invalidWeight = std::numeric_limits<double>::quiet_NaN();

Structure::Edge::Edge(Vertex* vertexA, Vertex* vertexB, const double& weightForward, const double& weightBackward)
   : vertexA(vertexA)
   , vertexB(vertexB)
   , weightForward(weightForward)
   , weightBackward(weightBackward)
{
}

Structure::Edge::~Edge()
{
}

double Structure::Edge::getForwardWeight() const
{
   return weightForward;
}

double Structure::Edge::getBackwardWeight() const
{
   return weightBackward;
}

bool Structure::Edge::hasForwardLink() const
{
   return !std::isnan(getForwardWeight());
}

bool Structure::Edge::hasBackwardLink() const
{
   return !std::isnan(getBackwardWeight());
}

bool Structure::Edge::linksVertex(const Vertex* vertex) const
{
   if (vertex == vertexA)
      return true;
   else if (vertex == vertexB)
      return true;

   return false;
}

const Structure::Vertex* Structure::Edge::getVertexA() const
{
   return vertexA;
}

const Structure::Vertex* Structure::Edge::getVertexB() const
{
   return vertexB;
}
