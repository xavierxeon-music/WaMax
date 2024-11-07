#include "../SDULibGraphEdgeSimple.h"

SDULibGraph::Edge::Simple::Simple(Vertex* vertexA, Vertex* vertexB, const double& weightForward, const double& weightBackward)
   : Abstract(vertexA, vertexB)
   , weightForward(weightForward)
   , weightBackward(weightBackward)
{
}

double SDULibGraph::Edge::Simple::getForwardWeight() const
{
   return weightForward;
}

double SDULibGraph::Edge::Simple::getBackwardWeight() const
{
   return weightBackward;
}
