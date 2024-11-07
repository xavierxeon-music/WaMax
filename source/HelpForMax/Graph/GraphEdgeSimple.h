#ifndef SDULibGraphEdgeSimpleH
#define SDULibGraphEdgeSimpleH

#include "SDULibGraphEdgeAbstract.h"
#include "SDULibGraphExportDef.h"

namespace SDULibGraph
{
   namespace Edge
   {
      class SDULIBGRAPH_DECLSPEC Simple : public Abstract
      {
      public:
         Simple(Vertex* vertexA, Vertex* vertexB, const double& weightForward = 1.0, const double& weightBackward = Abstract::invalidWeight);

      public:
         virtual double getForwardWeight() const override;
         virtual double getBackwardWeight() const override;

      private:
         double weightForward;
         double weightBackward;
      };
   } // namespace Edge
} // namespace SDULibGraph

#endif // NOT SDULibGraphEdgeSimpleH
