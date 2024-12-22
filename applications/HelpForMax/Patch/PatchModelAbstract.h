#ifndef PatchModelAbstractH
#define PatchModelAbstractH

#include <QStandardItemModel>

#include "PatchRefStructure.h"

namespace Patch
{
   namespace Model
   {
      class Abstract : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, RefStructure* structure, const RefStructure::PatchPart& part);
         ~Abstract();

      signals:
         void signalDataEdited();
         void signalUpdateDigest(const QModelIndex& index);

      public:
         virtual void update() = 0;
         virtual void rebuild() = 0;
         virtual RefStructure::Digest* getDigest(const QModelIndex& index) = 0;
         virtual void createBeforeItem(const QModelIndex& index);
         virtual void removeItem(const QModelIndex& index);
         const RefStructure::PatchPart& getPart() const;

         static void updateAll();
         static void rebuildAll();

      protected:
         void updateDigestItem(QStandardItem* digestItem, const RefStructure::Digest& digest);

      protected:
         RefStructure* structure;
         RefStructure::PatchPart part;

      private:
         static QList<Abstract*> instanceList;
      };
   } // namespace Model
} // namespace Patch

#endif // NOT PatchModelAbstractH
