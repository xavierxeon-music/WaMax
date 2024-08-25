#ifndef PackageInfoH
#define PackageInfoH

#include <QFileInfo>
#include <QString>

namespace Package
{
   class TabWidget;

   class Info
   {
   public:
      QString extractPatchName(const QString& patchFileName) const;
      const QString& getPath() const;
      const QString& getName() const;
      const QString& getAuthor() const;

   private:
      friend class TabWidget;

   private:
      Info(const QString& path);

   private:
      QString path;
      QString name;
      QString author;
   };
} // namespace Package

#endif // NOT PackageInfoH
