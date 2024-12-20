#ifndef SuggestModelAbstractH
#define SuggestModelAbstractH

#include <QStandardItemModel>

#include "PatchRefStructure.h"

namespace Suggest
{
   namespace Model
   {
      class Abstract : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Patch::RefStructure* structure, const Patch::RefStructure::PatchPart& part);

      protected:
         Patch::RefStructure* structure;
         Patch::RefStructure::PatchPart part;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelAbstractH
