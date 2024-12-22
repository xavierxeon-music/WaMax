#ifndef SuggestModelAbstractH
#define SuggestModelAbstractH

#include <QStandardItemModel>

#include "RefStructure.h"

namespace Suggest
{
   namespace Model
   {
      class Abstract : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part);

      protected:
         Ref::Structure& structure;
         Ref::Structure::PatchPart part;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelAbstractH
