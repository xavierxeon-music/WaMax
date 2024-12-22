#ifndef SuggestModelTypedMessageH
#define SuggestModelTypedMessageH

#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class TypedMessage : public Abstract
      {
         Q_OBJECT

      public:
         TypedMessage(QObject* parent, Ref::Structure& structure);
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelTypedMessageH
