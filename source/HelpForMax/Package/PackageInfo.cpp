#include "PackageInfo.h"

#include <QDir>

QString Package::Info::extractPatchName(const QString& patchFileName) const
{
   QFileInfo patchInfo(patchFileName);
   const QString patchName = patchInfo.fileName().replace(".maxpat", "");

   return patchName;
}

const QString& Package::Info::getPath() const
{
   return path;
}

const QString& Package::Info::getName() const
{
   return name;
}

const QString& Package::Info::getAuthor() const
{
   return author;
}

Package::Info::Info(const QString& path)
   : path(path)
   , name()
   , author()
{
}
