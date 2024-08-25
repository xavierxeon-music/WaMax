#ifndef PatchModelTypedMessageH
#define PatchModelTypedMessageH

#include "PatchModelAbstract.h"

namespace Patch
{
   namespace Model
   {
      class TypedMessage : public Abstract
      {
         Q_OBJECT

      public:
         TypedMessage(QObject* parent, Structure* structure);

      private:
         void update() override;
         void rebuild() override;
         Structure::Digest* getDigest(const QModelIndex& index) override;
         bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
      };
   } // namespace Model
} // namespace Patch

#endif // NOT PatchModelTypedMessageH
