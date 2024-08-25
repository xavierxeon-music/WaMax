#ifndef AppHelperH
#define AppHelperH

#include <QString>
#include <QStringList>

struct Helper
{
   enum class OpenState
   {
      Unkown,
      FileNotExist,
      AlreadyOpen,
      Done
   };

   static void launch(const QString& appPath, const QStringList& arguments = QStringList());
   static void python(const QStringList& arguments = QStringList());
   static OpenState openFile(const QString& path, const QString& appName);
   static void openApp(const QString& appName);
   static QStringList getAppWindowTitles(const QString& appName);
};

#endif // NOT AppHelperH
