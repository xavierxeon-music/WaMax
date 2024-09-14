#include "ProcessWrapper.h"

#include <functional>

#include <QThread>

ProcessWrapper::ProcessWrapper()
   : process()
{
   QObject::connect(&process, &QProcess::readyReadStandardOutput, std::bind(&ProcessWrapper::passStdOutput, this));
   QObject::connect(&process, &QProcess::readyReadStandardError, std::bind(&ProcessWrapper::passStdError, this));
}

void ProcessWrapper::launchDetached(const QString& appPath, const QStringList& arguments)
{
   process.startDetached(appPath, arguments);
   QThread::sleep(1);
}

void ProcessWrapper::write(const QStringList& data)
{
   for (const QString& part : data)
      process.write(part.toUtf8() + "\n");
}

bool ProcessWrapper::launch(const QString& appPath, const QStringList& arguments)
{
   process.start(appPath, arguments);

   if (!process.waitForStarted())
      return false;

   return true;
}

QString ProcessWrapper::clean(const QString& name) const
{
   const int slashIndex = name.indexOf("/", 1);
   const QString colonTest = (-1 != slashIndex) ? name.mid(slashIndex - 1, 1) : "";
   if (":" != colonTest)
      return name;

   const QString front = name.mid(0, slashIndex - 1);
   const QString back = name.mid(slashIndex);

   return "/Volumes/" + front + back;
}

void ProcessWrapper::passStdOutput()
{
   stdOutput(QString::fromUtf8(process.readAllStandardOutput()));
}

void ProcessWrapper::passStdError()
{
   stdError(QString::fromUtf8(process.readAllStandardError()));
}
