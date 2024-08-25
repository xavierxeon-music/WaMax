#include "PackageTabWidget.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include "HelpForMax.h"
#include "MessageBar.h"
#include "PackageInfo.h"
#include "PackageWidget.h"

Package::TabWidget* Package::TabWidget::me = nullptr;

Package::TabWidget::TabWidget(QWidget* parent)
   : RecentTabWidget(parent, "Package")
   , infoMap()
   , server(nullptr)
   , socket()
   , linkEnabled(false)
   , linkAction(nullptr)
   , linkMap()
{
   me = this;

   linkMap[true] = QIcon(":/MaxLinkActive.svg");
   linkMap[false] = QIcon(":/MaxLinkInactive.svg");

   server = new QLocalServer(this);
   connect(server, &QLocalServer::newConnection, this, &TabWidget::slotNewConnection);
   qDebug() << "Server @" << HelpForMax::compileSockerName();
   server->listen(HelpForMax::compileSockerName());
}

Package::TabWidget::~TabWidget()
{
   me = nullptr;
}

void Package::TabWidget::slotRefWritten(const QString& patchPath)
{
   if (socket.isNull())
      return;

   QJsonObject object;
   object["patch"] = patchPath;
   qDebug() << "SEND" << object;

   QJsonDocument doc(object);
   socket->write(doc.toJson(QJsonDocument::Compact));
}

void Package::TabWidget::populate(QMenu* packageMenu, QToolBar* toolBar)
{
   //
   packageMenu->addAction(QIcon(":/PackageLoad.svg"), "Load", this, &TabWidget::slotLoadPackage);
   linkAction = packageMenu->addAction(linkMap.value(false), "Link", this, &TabWidget::slotLinkToMax);
   linkAction->setCheckable(true);

   QSettings settings;
   linkEnabled = settings.value("Package/Link").toBool();
   linkAction->setChecked(linkEnabled);

   packageMenu->addSeparator();
   packageMenu->addMenu(getRecentMenu());

   packageMenu->addSeparator();
   packageMenu->addAction(QIcon(":/PackageClose.svg"), "Close", this, &TabWidget::slotClosePackage);

   //
   toolBar->addAction(linkAction);
}

Package::Info* Package::TabWidget::findOrCreate(const QString& someFileInPackage)
{
   if (!me)
      return nullptr;

   QFileInfo patchInfo(someFileInPackage);

   if (!patchInfo.exists())
      return nullptr;

   QString path = [&]()
   {
      for (QDir dir = patchInfo.dir(); !dir.isRoot(); dir.cdUp())
      {
         const QFileInfoList content = dir.entryInfoList(QDir::Files);
         for (const QFileInfo& contentInfo : content)
         {
            if ("package-info.json" != contentInfo.fileName())
               continue;

            return dir.absolutePath();
         }
      }

      return QString();
   }();

   if (path.isEmpty())
      return nullptr;

   return me->get(path);
}

Package::Info* Package::TabWidget::get(const QString& packagePath)
{
   if (infoMap.contains(packagePath))
      return infoMap.value(packagePath);

   QFile file(packagePath + "/package-info.json");
   if (!file.open(QIODevice::ReadOnly))
      return nullptr;

   Info* info = new Info(packagePath);
   infoMap[packagePath] = info;

   const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
   file.close();

   const QJsonObject object = doc.object();
   if (!object.empty())
   {
      info->author = object["author"].toString();
      info->name = object["name"].toString();

      Message::Bar::message() << "LOADED PACKAGE " << info->name;
   }

   Widget* view = new Widget(this, info);
   connect(view, &Widget::signalPatchSeleted, this, &TabWidget::signalPatchSeleted);

   addTab(view, info->name);
   addRecentFile(info->path);
   addCurrentFile(info->path);

   return info;
}

void Package::TabWidget::init()
{
   for (const QString& packagePath : getCurrrentFiles())
   {
      findOrCreate(packagePath + "/");
   }
}

void Package::TabWidget::slotLoadPackage()
{
   const QString packageFileName = QFileDialog::getOpenFileName(this, "MAX PACKAGE", QString(), "package-info.json");
   if (packageFileName.isEmpty())
      return;

   findOrCreate(packageFileName);
}

void Package::TabWidget::slotClosePackage()
{
   Widget* view = qobject_cast<Widget*>(currentWidget());
   const Info* info = view->getPackageInfo();
   view->deleteLater();

   removeCurrentFile(info->path);
   emit signalCloseAllPatches(info);
}

void Package::TabWidget::slotNewConnection()
{
   socket = server->nextPendingConnection();

   auto setLinkActive = [this](bool active)
   {
      QIcon icon = this->linkMap.value(active);
      this->linkAction->setIcon(icon);
   };

   auto inactiveFunction = std::bind(setLinkActive, false);
   connect(socket, &QLocalSocket::disconnected, inactiveFunction);

   connect(socket, &QIODevice::readyRead, this, &TabWidget::slotReceiveSocket);

   setLinkActive(true);
}

void Package::TabWidget::slotReceiveSocket()
{
   if (!linkEnabled)
      return;

   const QJsonDocument doc = QJsonDocument::fromJson(socket->readAll());
   const QJsonObject object = doc.object();

   const QString path = object["patch"].toString();
   Info* info = findOrCreate(path);
   if (!info)
      return;

   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* view = qobject_cast<Widget*>(widget(index));
      if (view->getPackageInfo() != info)
         continue;

      setCurrentIndex(index);
      view->selectPatch(path);
      break;
   }
}

void Package::TabWidget::slotLinkToMax(bool enabled)
{
   linkEnabled = enabled;

   QSettings settings;
   settings.setValue("Package/Link", linkEnabled);
}

RecentTabWidget::Entry Package::TabWidget::creatreEntry(const QFileInfo& fileInfo)
{
   const QString name = fileInfo.baseName();
   auto openFunction = [this, fileInfo]()
   {
      const QString packagePath = fileInfo.absoluteFilePath() + "/";
      this->findOrCreate(packagePath);
   };

   Entry entry{name, openFunction};
   return entry;
}
