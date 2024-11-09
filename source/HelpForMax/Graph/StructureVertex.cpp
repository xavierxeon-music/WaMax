#include "StructureVertex.h"

Structure::Vertex::Vertex(const QString& name)
   : name(name)
{
}

const QString& Structure::Vertex::getName() const
{
   return name;
}
