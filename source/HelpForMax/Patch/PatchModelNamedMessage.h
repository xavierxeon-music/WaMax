#ifndef PatchModelNamedMessageH
#define PatchModelNamedMessageH

#include "DelegateDataType.h"
#include "PatchModelAbstract.h"

namespace Patch
{
   namespace Model
   {
      class NamedMessage : public Abstract, public Delegate::DataType::Source
      {
         Q_OBJECT

      public:
         NamedMessage(QObject* parent, RefStructure* structure);

      private:
         void update() override;
         void rebuild() override;
         RefStructure::Digest* getDigest(const QModelIndex& index) override;
         void createBeforeItem(const QModelIndex& index) override;
         void removeItem(const QModelIndex& index) override;
         bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
         Max::DataType getDataType(const int index) override;
      };
   } // namespace Model
} // namespace Patch

#endif // NOT PatchModelNamedMessageH
