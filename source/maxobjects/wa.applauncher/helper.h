#ifndef AppHelperH
#define AppHelperH

#include <QString>
#include <QStringList>

#include <QProcess>

class Helper
{
public:
   enum class OpenState
   {
      Unkown,
      FileNotExist,
      AlreadyOpen,
      Done
   };

public:
   Helper();

public: // general
   void launchDetached(const QString& appPath, const QStringList& arguments = QStringList());

public: //os spectific
   void python(const QStringList& arguments = QStringList());
   OpenState openFileWithApp(const QString& path, const QString& appName);
   void openApp(const QString& appName);
   QStringList getAppWindowTitles(const QString& appName);

private:
   QString clean(const QString& name) const;

private:
   QProcess process;
};

#endif // NOT AppHelperH
