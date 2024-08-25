#ifndef PackageModelH
#define PackageModelH

#include <QStandardItemModel>

namespace Package
{
   class Info;

   class Model : public QStandardItemModel
   {
      Q_OBJECT

   public:
      Model(QObject* parent);

   public:
      void create(const Info* info);
      QModelIndex find(const QString& patchFileName) const;
   };
} // namespace Package

#endif // NOT PackageModelH
