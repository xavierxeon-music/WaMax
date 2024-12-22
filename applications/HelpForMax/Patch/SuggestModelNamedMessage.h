#ifndef SuggestModelNamedMessageH
#define SuggestModelNamedMessageH

#include "DelegateDataType.h"
#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class NamedMessage : public Abstract, public Delegate::DataType::Source
      {
         Q_OBJECT

      public:
         NamedMessage(QObject* parent, Ref::Structure& structure);

      private:
         Max::DataType getDataType(const int index) override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelNamedMessageH
