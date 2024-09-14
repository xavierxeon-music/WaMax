#include "wa.applauncher.h"

#include <QDir>
#include <QFileInfo>
#include <QHostInfo>

#include <patcher.h>

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <libgen.h>
#endif

QString AppLauncher::packagePath;

AppLauncher::AppLauncher(const atoms& args)
   : object<AppLauncher>()
   , Helper()
   , inputMessage(this, "messages")
   , inputStdIn(this, "std in")
   , outputRequest(this, "message")
   , outputStdOut(this, "std out")
   , outputStdErr(this, "std in")
   , openFile(this, "openFile", "open file", Patcher::minBind(this, &AppLauncher::openFileFunction))
   , openApp(this, "openApp", "open application", Patcher::minBind(this, &AppLauncher::openAppFunction))
   , lanch(this, "lanch", "launch application", Patcher::minBind(this, &AppLauncher::launchFunction))
   , python(this, "python", "open python script", Patcher::minBind(this, &AppLauncher::pyhonFunction))
   , anything(this, "anything", "std input", Patcher::minBind(this, &AppLauncher::stdinFunction))
   , package(this, "package", "get the package path", Patcher::minBind(this, &AppLauncher::packagePathFunction))
   , hostname(this, "hostname", "get hostname", Patcher::minBind(this, &AppLauncher::hostnameFunction))
{
}

void AppLauncher::setPackagePath(const char* external_path)
{
   QFileInfo patchInfo(external_path);
   for (QDir dir = patchInfo.dir(); !dir.isRoot(); dir.cdUp())
   {
      const QFileInfoList content = dir.entryInfoList(QDir::Files);
      for (const QFileInfo& contentInfo : content)
      {
         if ("package-info.json" != contentInfo.fileName())
            continue;

         packagePath = contentInfo.dir().absolutePath();
         break;
      }
   }
}

atoms AppLauncher::openFileFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   if (1 > args.size())
      return {};

   const std::string fileName = args[0];
   const std::string appName = (2 == args.size()) ? args[1] : "";
   const OpenState state = openFileWithApp(QString::fromStdString(fileName), QString::fromStdString(appName));

   if (state == OpenState::FileNotExist)
      cerr << fileName << " does not exist" << endl;
   else if (state == OpenState::AlreadyOpen)
      cout << fileName << " already open in " << appName << endl;
   else if (state == OpenState::Done)
      cout << fileName << " opened in " << appName << endl;

   return {};
}

atoms AppLauncher::openAppFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   if (1 > args.size())
      return {};

   const std::string appName = args[0];
   openApp(QString::fromStdString(appName));

   return {};
}

atoms AppLauncher::launchFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   if (1 > args.size())
      return {};

   QString appName;
   QStringList argumentList;
   for (size_t i = 0; i < args.size(); i++)
   {
      const QString argument = QString::fromStdString(args[i]);
      if (0 == i)
         appName = argument;
      else
         argumentList.append(argument);
   }

   launchDetached(appName, argumentList);

   return {};
}

atoms AppLauncher::pyhonFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   if (1 > args.size())
      return {};

   QStringList argumentList;
   for (auto i = 0; i < args.size(); i++)
   {
      const QString argument = QString::fromStdString(args[i]);
      argumentList.append(argument);
   }

   python(argumentList);

   return {};
}

atoms AppLauncher::stdinFunction(const atoms& args, const int inlet)
{
   if (1 != inlet)
      return {};

   QStringList data;
   for (auto i = 0; i < args.size(); i++)
   {
      const QString argument = QString::fromStdString(args[i]);
      data.append(argument);
   }

   write(data);

   return {};
}

atoms AppLauncher::packagePathFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   outputRequest.send(packagePath.toStdString());
   return {};
}

atoms AppLauncher::hostnameFunction(const atoms& args, const int inlet)
{
   if (0 != inlet)
      return {};

   outputRequest.send(QHostInfo::localHostName().toStdString());
   return {};
}

void AppLauncher::stdOutput(const QString& text)
{
   outputStdOut.send(text.toStdString());
}

void AppLauncher::stdError(const QString& text)
{
   outputStdErr.send(text.toStdString());
}

// main

void ext_main(void* moduleRef)
{
   c74::min::wrap_as_max_external<AppLauncher>("AppLauncher", __FILE__, moduleRef);

#if defined(__APPLE__)
   CFURLRef ext_url_ref = CFBundleCopyBundleURL((CFBundleRef)moduleRef);
   CFStringRef mac_path = CFURLCopyFileSystemPath(ext_url_ref, kCFURLPOSIXPathStyle);
   const char* external_path = CFStringGetCStringPtr(mac_path, CFStringGetSystemEncoding());

   AppLauncher::setPackagePath(external_path);

   CFRelease(ext_url_ref);
   CFRelease(mac_path);
#else
   const char* external_path[256];
   GetModuleFileName(moduleRef, (LPCH)external_path, sizeof(external_path));

   AppLauncher::setPackagePath(external_path);
#endif
}
