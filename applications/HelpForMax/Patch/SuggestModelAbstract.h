#ifndef SuggestModelAbstractH
#define SuggestModelAbstractH

#include <QStandardItemModel>

#include "MaxRefStructure.h"

namespace Suggest
{
   namespace Model
   {
      class Abstract : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Max::RefStructure& structure, const Max::RefStructure::PatchPart& part);

      protected:
         Max::RefStructure& structure;
         Max::RefStructure::PatchPart part;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelAbstractH
