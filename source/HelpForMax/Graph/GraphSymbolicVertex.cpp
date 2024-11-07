#include "GraphSymbolicVertex.h"

Graph::Symbolic::Vertex::Vertex(const QString& name)
   : name(name)
{
}

const QString& Graph::Symbolic::Vertex::getName() const
{
   return name;
}
