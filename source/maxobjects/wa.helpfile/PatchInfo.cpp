#include "PatchInfo.h"

#include <QDir>
#include <QFileInfo>

PatchInfo::PatchInfo(const QString& patchPath)
   : state(State::Initial)
   , patchPath(patchPath)
   , helpPath()
   , packagePath()
{
}

bool PatchInfo::checkState()
{
   if (patchPath.isEmpty())
      return false;

   const QFileInfo patchInfo(patchPath);
   if (State::Initial == state)
   {
      state = State::NotInPackage;

      if (!patchInfo.exists())
         return false;

      const QString patchName = patchInfo.fileName().replace(".maxpat", "");
      for (QDir dir = patchInfo.dir(); !dir.isRoot(); dir.cdUp())
      {
         const QFileInfoList content = dir.entryInfoList(QDir::Files);
         for (const QFileInfo& contentInfo : content)
         {
            if ("package-info.json" != contentInfo.fileName())
               continue;

            // package info found
            packagePath = dir.absolutePath();
            helpPath = packagePath + "/docs/" + patchName + ".maxref.xml";
            state = State::HelpFileOutdated;

            break;
         }
      }
   }

   if (State::NotInPackage == state)
      return true;

   QFileInfo refInfo(packagePath);

   if (refInfo.lastModified() > patchInfo.lastModified())
      state = State::UpToDate;
   else
      state = State::HelpFileOutdated;

   return true;
}
