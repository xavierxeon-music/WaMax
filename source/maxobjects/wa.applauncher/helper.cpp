#include "helper.h"

#include <functional>

#include <QThread>

Helper::Helper()
   : process()
{
   QObject::connect(&process, &QProcess::readyReadStandardOutput, std::bind(&Helper::passStdOutput, this));
   QObject::connect(&process, &QProcess::readyReadStandardError, std::bind(&Helper::passStdError, this));
}

void Helper::launchDetached(const QString& appPath, const QStringList& arguments)
{
   process.startDetached(appPath, arguments);
   QThread::sleep(1);
}

void Helper::write(const QStringList& data)
{
   for (const QString& part : data)
      process.write(part.toUtf8() + "\n");
}

bool Helper::launch(const QString& appPath, const QStringList& arguments)
{
   process.start(appPath, arguments);

   if (!process.waitForStarted())
      return false;

   return true;
}

QString Helper::clean(const QString& name) const
{
   const int slashIndex = name.indexOf("/", 1);
   const QString colonTest = (-1 != slashIndex) ? name.mid(slashIndex - 1, 1) : "";
   if (":" != colonTest)
      return name;

   const QString front = name.mid(0, slashIndex - 1);
   const QString back = name.mid(slashIndex);

   return "/Volumes/" + front + back;
}

void Helper::passStdOutput()
{
   stdOutput(QString::fromUtf8(process.readAllStandardOutput()));
}

void Helper::passStdError()
{
   stdError(QString::fromUtf8(process.readAllStandardError()));
}
