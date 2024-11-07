#ifndef SDULibGraphVertexH
#define SDULibGraphVertexH

#include "SDULibGraphExportDef.h"

#include <QList>

namespace SDULibGraph
{
   class SDULIBGRAPH_DECLSPEC Vertex
   {
   public:
      using PrtList = QList<Vertex*>;
      using ConstPtrList = QList<const Vertex*>;

   public:
      Vertex();
   };
} // namespace SDULibGraph

#endif // NOT SDULibGraphVertexH
