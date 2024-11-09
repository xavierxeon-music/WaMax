#include "PatchTabWidget.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMenu>
#include <QSettings>
#include <QTimer>

#include "MainWindow.h"
#include "MessageBar.h"
#include "PackageInfo.h"
#include "PackageTabWidget.h"
#include "PatchWidget.h"

Patch::TabWidget::TabWidget(QWidget* parent)
   : RecentTabWidget(parent, "Patch")
{
   connect(this, &QTabWidget::currentChanged, this, &TabWidget::slotTabChanged);
}

void Patch::TabWidget::populate(QMenu* patchMenu, QMenu* viewMenu, QToolBar* toolBar)
{
   //
   patchMenu->addAction(QIcon(":/PatchLoad.svg"), "Load", this, &TabWidget::slotPromptLoadPatch);

   QAction* saveAction = patchMenu->addAction(QIcon(":/PatchSave.svg"), "Save", this, &TabWidget::slotWriteRef);
   saveAction->setShortcut(QKeySequence::Save);

   QAction* saveAllAction = patchMenu->addAction(QIcon(":/PatchSaveAll.svg"), "Save All", this, &TabWidget::slotWriteAllRefs);
   saveAllAction->setShortcut(Qt::ShiftModifier | Qt::ControlModifier | Qt::Key_S);
   patchMenu->addSeparator();

   patchMenu->addMenu(getRecentMenu());
   patchMenu->addSeparator();

   QAction* closePatchAction = patchMenu->addAction(QIcon(":/PatchClose.svg"), "Close", this, &TabWidget::slotClosePatch);
   closePatchAction->setShortcut(QKeySequence::Close);

   //

   //
   viewMenu->addSeparator();
   QAction* openInMaxAction = viewMenu->addAction(QIcon(":/PatchOpenInMax.svg"), "Open In Max", this, &TabWidget::slotOpenInMax);
   openInMaxAction->setShortcuts(QKeySequence::Italic);

   QAction* showXMLAction = viewMenu->addAction(QIcon(":/PatchOpenRef.svg"), "Open XML", this, &TabWidget::slotOpenXML);
   showXMLAction->setShortcut(QKeySequence::Open);
   viewMenu->addSeparator();

   //
   toolBar->addAction(saveAction);
   toolBar->addSeparator();
   toolBar->addAction(closePatchAction);
}

void Patch::TabWidget::init()
{
   for (const QString& patchPath : getCurrrentFiles())
   {
      slotShowPatch(patchPath);
   }
}

void Patch::TabWidget::emitSignalCheckDirty()
{
   QTimer::singleShot(200, this, &TabWidget::updateTabNames);
}

void Patch::TabWidget::slotPromptLoadPatch()
{
   const QString patchFileName = QFileDialog::getOpenFileName(this, "MAX PATCH", QString(), "*.maxpat");
   if (patchFileName.isEmpty())
      return;

   slotShowPatch(patchFileName);
}

void Patch::TabWidget::slotShowPatch(const QString& patchFileName)
{
   Package::Info* info = Package::TabWidget::findOrCreate(patchFileName);
   if (!info)
   {
      Message::Bar::warning() << "PATCH does not belong to a package" << patchFileName;
      this->deleteLater();
      return;
   }

   const Patch::Info patchInfo = info->extractPatchInfo(patchFileName);
   for (int index = 0; index < tabBar()->count(); index++)
   {
      if (patchInfo.name == tabText(index))
      {
         setCurrentIndex(index);
         return;
      }
   }

   Widget* patchWidget = new Patch::Widget(this, info, patchFileName);
   const int index = addTab(patchWidget, patchInfo.name);
   setCurrentIndex(index);

   addRecentFile(patchFileName);
   addCurrentFile(patchFileName);
}

void Patch::TabWidget::slotWriteRef()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (!patchWidget)
      return;

   emit signalRefWritten(patchWidget->getPath());
   patchWidget->writeRef();
}

void Patch::TabWidget::slotWriteAllRefs()
{
   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (!patchWidget)
         continue;

      patchWidget->writeRef();
      emit signalRefWritten(patchWidget->getPath());
   }
}

void Patch::TabWidget::slotClosePatch()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (!patchWidget)
      return;

   removeCurrentFile(patchWidget->getPath());
   patchWidget->deleteLater();
}

void Patch::TabWidget::slotCloseAllPatches(const Package::Info* packageInfo)
{
   QList<Widget*> deleteList;
   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (packageInfo && packageInfo != patchWidget->getPacakgeInfo())
         continue;

      deleteList.append(patchWidget);
   }

   for (Widget* patchWidget : deleteList)
   {
      removeCurrentFile(patchWidget->getPath());
      patchWidget->deleteLater();
   }
}

void Patch::TabWidget::slotShowSuggestions(bool enabled)
{
}

void Patch::TabWidget::slotOpenInMax()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (patchWidget)
      patchWidget->openInMax();
}

void Patch::TabWidget::slotOpenXML()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (patchWidget)
      patchWidget->openXML();
}

void Patch::TabWidget::slotTabChanged(int index)
{
   if (index < 0)
   {
      // no tab left
      emit signalTabSelected(nullptr);
      return;
   }

   Widget* patchWidget = qobject_cast<Widget*>(widget(index));
   emit signalTabSelected(patchWidget);
}

RecentTabWidget::Entry Patch::TabWidget::creatreEntry(const QFileInfo& fileInfo)
{
   const QString patchName = fileInfo.completeBaseName();
   auto openFunction = std::bind(&TabWidget::slotShowPatch, this, fileInfo.absoluteFilePath());

   Entry entry{patchName, openFunction};
   return entry;
}

void Patch::TabWidget::updateTabNames()
{
   static const QString bullet = QString::fromUtf8("\u25cf") + QString(" ");

   for (int index = 0; index < tabBar()->count(); index++)
   {
      QWidget* content = widget(index);
      Widget* patchWidget = qobject_cast<Widget*>(content);
      if (!patchWidget)
         continue;

      QString name = patchWidget->getPatchInfo().name;
      if (patchWidget->isDirty())
         name = bullet + name;

      setTabText(index, name);
   }

   emit signalCheckDirty();
}
