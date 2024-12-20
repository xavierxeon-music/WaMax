#ifndef SuggestModelOutputH
#define SuggestModelOutputH

#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class Output : public Abstract
      {
         Q_OBJECT

      public:
         Output(QObject* parent, Patch::RefStructure* structure);
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelOutputH
