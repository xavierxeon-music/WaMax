#include "ProcessWrapper.h"

void ProcessWrapper::python(const QStringList& arguments)
{
}

ProcessWrapper::OpenState ProcessWrapper::openFileWithApp(const QString& path, const QString& appName)
{
   return OpenState::Unkown;
}

void ProcessWrapper::openNoFileApp(const QString& appName)
{
}

QStringList ProcessWrapper::getAppWindowTitles(const QString& appName)
{
   QStringList titles;
   return titles;
}
