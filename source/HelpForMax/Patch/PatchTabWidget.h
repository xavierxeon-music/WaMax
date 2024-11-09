#ifndef PatchTabWidgetH
#define PatchTabWidgetH

#include "RecentTabWidget.h"

#include <QMenu>
#include <QToolBar>

#include "MaxPatcher.h"

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

   signals:
      void signalCheckDirty();

   public:
      void populate(QMenu* patchMenu, QMenu* viewMenu, QToolBar* toolBar);
      void init();
      void emitSignalCheckDirty();

   signals:
      void signalTabSelected(Max::Patcher* patcher);
      void signalRefWritten(const QString& patchPath);

   public slots:
      void slotShowPatch(const QString& patchFileName);
      void slotCloseAllPatches(const Package::Info* packageInfo);
      void slotShowSuggestions(bool enabled);

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
      void updateTabNames();
   };
} // namespace Patch

#endif // NOT PatchTabWidgetH
