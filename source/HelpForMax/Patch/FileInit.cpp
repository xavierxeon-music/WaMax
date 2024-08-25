#include "FileInit.h"

#include "Package/PackageInfo.h"

File::Init::Init(Patch::Structure* structure, const Package::Info* info)
   : Abstract(structure, info)
{
}

void File::Init::write(const QString& patchName)
{
   const QString initPath = getFilePath(patchName);

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
         stream << "max objectfile " << patchName << " " << patchName << ";\n";
         stream << "max definesubstitution " << patchName << " bpatcher @name " << patchName << ".maxpat;\n";
      }
      else if (structure->header.patcherType == Patch::Structure::PatchType::Poly)
      {
         stream << "max objectfile " << patchName << " " << patchName << ";\n";
         stream << "max definesubstitution " << patchName << " poly~ " << patchName << " 16;\n";
      }
      file.close();
   }
}

QString File::Init::getFilePath(const QString& patchName)
{
   const QString initPath = info->getPath() + "/init/" + patchName + ".txt";
   return initPath;
}
