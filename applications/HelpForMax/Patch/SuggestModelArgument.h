#ifndef SuggestModelArgumentH
#define SuggestModelArgumentH

#include "DelegateDataType.h"
#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class Argument : public Abstract, public Delegate::DataType::Source
      {
         Q_OBJECT

      public:
         Argument(QObject* parent, Max::RefStructure& structure);

      private:
         Max::DataType getDataType(const int index) override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelArgumentH
