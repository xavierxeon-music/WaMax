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
   bool launch(const QString& appPath, const QStringList& arguments = QStringList());
   void write(const QStringList& data = QStringList());

public: //os spectific
   void python(const QStringList& arguments = QStringList());
   OpenState openFileWithApp(const QString& path, const QString& appName);
   void openApp(const QString& appName);
   QStringList getAppWindowTitles(const QString& appName);

protected:
   virtual void stdOutput(const QString& text) = 0;
   virtual void stdError(const QString& text) = 0;

private:
   QString clean(const QString& name) const;
   void passStdOutput();
   void passStdError();

private:
   QProcess process;
};

#endif // NOT AppHelperH
