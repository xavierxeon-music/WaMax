#ifndef PatchModelHeaderH
#define PatchModelHeaderH

#include "DelegatePatchType.h"
#include "PatchModelAbstract.h"

namespace Patch
{
   namespace Model
   {
      class Header : public Abstract, public Delegate::PatchType::Source
      {
         Q_OBJECT

      public:
         Header(QObject* parent, RefStructure* structure);

      private:
         void update() override;
         void rebuild() override;
         RefStructure::Digest* getDigest(const QModelIndex& index) override;
         bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
         Patch::RefStructure::PatchType getPatchType(const int index) override;
      };
   } // namespace Model
} // namespace Patch

#endif // NOT PatchModelHeaderH
