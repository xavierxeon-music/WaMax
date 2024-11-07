#ifndef SDULibGraphNamedVertexH
#define SDULibGraphNamedVertexH

#include "SDULibGraphExportDef.h"
#include "SDULibGraphVertex.h"

namespace SDULibGraph
{
   class SDULIBGRAPH_DECLSPEC NamedVertex : public Vertex
   {
   public:
      NamedVertex(const QString& name);

   public:
      virtual const QString& getName() const;

   private:
      QString name;
   };
} // namespace SDULibGraph

#endif // NOT SDULibGraphNamedVertexH
