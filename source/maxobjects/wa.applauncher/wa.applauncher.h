#ifndef WaAppLauncherH
#define WaAppLauncherH

#include "c74_min.h"
using namespace c74::min;

#include <QString>

#include "helper.h"

class AppLauncher : public object<AppLauncher>, public Helper
{
public:
   MIN_DESCRIPTION{"launch an application"};

public:
   AppLauncher(const atoms& args = {});

public:
   inlet<> inputMessage;
   inlet<> inputStdIn;

   outlet<> outputRequest;
   outlet<> outputStdOut;
   outlet<> outputStdErr;

   message<> openFile;
   message<> openApp;
   message<> lanch;
   message<> python;
   message<> anything;
   message<> package;
   message<> hostname;

   static void setPackagePath(const char* external_path);

private:
   atoms openFileFunction(const atoms& args, const int inlet);
   atoms openAppFunction(const atoms& args, const int inlet);
   atoms launchFunction(const atoms& args, const int inlet);
   atoms pyhonFunction(const atoms& args, const int inlet);
   atoms stdinFunction(const atoms& args, const int inlet);
   atoms packagePathFunction(const atoms& args, const int inlet);
   atoms hostnameFunction(const atoms& args, const int inlet);

   void stdOutput(const QString& text) override;
   void stdError(const QString& text) override;

private:
   static QString packagePath;
};

#endif // NOT  WaAppLauncherH
