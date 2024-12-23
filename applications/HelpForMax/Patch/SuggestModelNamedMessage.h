#ifndef SuggestModelNamedMessageH
#define SuggestModelNamedMessageH

#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class NamedMessage : public Abstract
      {
         Q_OBJECT

      public:
         NamedMessage(QObject* parent, Ref::Structure& structure);

      private:
         void rebuild() override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelNamedMessageH
