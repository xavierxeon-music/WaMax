#include "GraphAbstractVertex.h"

Graph::Abstract::Vertex::Vertex(const QString& name)
   : name(name)
{
}

const QString& Graph::Abstract::Vertex::getName() const
{
   return name;
}
