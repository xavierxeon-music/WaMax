#ifndef PatchModelAbstractH
#define PatchModelAbstractH

#include <QStandardItemModel>

#include "PatchStructure.h"

namespace Patch
{
   namespace Model
   {
      class Abstract : public QStandardItemModel
      {
         Q_OBJECT
      public:
         using List = QList<Abstract*>;

      public:
         Abstract(QObject* parent, Structure* structure, const Structure::PatchPart& part);

      signals:
         void signalDataEdited();

      public:
         virtual void update() = 0;
         virtual void rebuild() = 0;
         virtual Structure::Digest* getDigest(const QModelIndex& index) = 0;
         virtual void createBeforeItem(const QModelIndex& index);
         virtual void removeItem(const QModelIndex& index);
         const Structure::PatchPart& getPart() const;

      protected:
         void updateDigestItem(QStandardItem* digestItem, const Structure::Digest& digest);

      protected:
         Structure* structure;
         Structure::PatchPart part;
      };
   } // namespace Model
} // namespace Patch

#endif // NOT PatchModelAbstractH
