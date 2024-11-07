#include "../SDULibGraphNamedVertex.h"

SDULibGraph::NamedVertex::NamedVertex(const QString& name)
   : Vertex()
   , name(name)
{
}

const QString& SDULibGraph::NamedVertex::getName() const
{
   return name;
}
