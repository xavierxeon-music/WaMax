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
      Model(QObject* parent, const Info* packageInfo);

   public:
      void create();
      void update();
      QModelIndex find(const QString& patchFileName) const;

   private:
      enum DataRoles
      {
         RolePath = Qt::UserRole + 1,
         RoleInfo = Qt::UserRole + 2

      };

   private:
      const Info* packageInfo;
   };
} // namespace Package

#endif // NOT PackageModelH
