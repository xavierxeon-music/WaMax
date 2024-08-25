#ifndef PatchTabWidgetH
#define PatchTabWidgetH

#include "RecentTabWidget.h"

#include <QMenu>
#include <QToolBar>

namespace Package
{
   class Info;
}

namespace Patch
{
   class Widget;

   class TabWidget : public RecentTabWidget
   {
      Q_OBJECT

   public:
      TabWidget(QWidget* parent);

   public:
      void populate(QMenu* patchMenu, QMenu* viewMenu, QToolBar* toolBar);
      void forceDirtyCheck();
      void init();

   signals:
      void signalTabSelected(const QString& patchPath, const Package::Info* info);
      void signalRefWritten(const QString& patchPath);

   public slots:
      void slotLoadPatch(const QString& patchFileName);
      void slotCloseAllPatches(const Package::Info* info);

   private slots:
      void slotPromptLoadPatch();
      void slotWriteRef();
      void slotClosePatch();
      void slotWriteAllRefs();
      void slotOpenInMax();
      void slotOpenXML();
      void slotTabChanged(int index);

   private:
      Entry creatreEntry(const QFileInfo& fileInfo) override;
   };
} // namespace Patch

#endif // NOT PatchTabWidgetH
