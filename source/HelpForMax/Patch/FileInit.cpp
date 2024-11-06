#include "FileInit.h"

#include "Package/PackageInfo.h"

File::Init::Init(Patch::Structure* structure, const Package::Info* info)
   : Abstract(structure, info)
{
}

void File::Init::write(const Patch::Info& patchInfo)
{
   const QString initPath = getFilePath(patchInfo);

   QFile file(initPath);

   if (structure->header.patcherType == Patch::Structure::PatchType::Standard) // delete file
   {
      if (!file.exists()) // nothing to delete
         return;

      file.remove();
   }
   else
   {
      if (!file.open(QIODevice::WriteOnly))
         return;

      QTextStream stream(&file);

      if (structure->header.patcherType == Patch::Structure::PatchType::Gui)
      {
         stream << "max objectfile " << patchInfo.name << " " << patchInfo.name << ";\n";
         stream << "max definesubstitution " << patchInfo.name << " bpatcher @name " << patchInfo.name << ".maxpat;\n";
      }
      else if (structure->header.patcherType == Patch::Structure::PatchType::Poly)
      {
         stream << "max objectfile " << patchInfo.name << " " << patchInfo.name << ";\n";
         stream << "max definesubstitution " << patchInfo.name << " poly~ " << patchInfo.name << " 16;\n";
      }
      file.close();
   }
}

QString File::Init::getFilePath(const Patch::Info& patchInfo)
{
   QString initPath = info->getPath() + "/init/";
   if (!patchInfo.folder.isEmpty())
      initPath += patchInfo.folder + "/";
   initPath += patchInfo.name + ".txt";

   return initPath;
}
