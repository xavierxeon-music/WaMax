#ifndef PatchWidgetH
#define PatchWidgetH

#include "../_Forms/ui_DigestWidget.h"
#include "../_Forms/ui_PatchWidget.h"
#include <QSplitter>

#include "MaxPatcher.h"
#include "MaxWidget.h"
#include "PatchInfo.h"
#include "PatchTabWidget.h"
#include "RefStructure.h"

namespace Package
{
   class Info;
}

namespace Patch
{
   class Widget : public QSplitter, private Ui::PatchWidget, private Ui::DigestWidget
   {
      Q_OBJECT

   public:
      Widget(TabWidget* tabWidget, const Package::Info* packageInfo, const QString& patchFileName);
      ~Widget();

   public:
      const QString& getPath() const;
      const Package::Info* getPacakgeInfo() const;
      const Patch::Info& getPatchInfo() const;
      virtual void writeRef();
      void openInMax();
      void openXML();
      bool isDirty() const;
      void setToolsVisible(TabWidget::ToolsVisible toolsVisible);

   private:
      friend class TreeView;

   private slots:
      void slotSetPatchDigest();
      void slotSaveDigestText();
      void slotSaveDigestDescription();

   private:
      void setDigest(Ref::Structure::Digest* newDigest, const Ref::Structure::PatchPart& part);
      void rebuild();
      void update();
      void setDirty();
      void propagateDirty(bool isDirty);
      void setIcon(QLabel* iconLabel, Ref::Structure::PatchPart part);

   private:
      Ref::Structure maxRef;
      Max::Patcher maxPatch;

      TabWidget* tabWidget;
      Max::Widget* structureWidget;

      const Package::Info* packageInfo;
      QString path;
      Patch::Info patchInfo;

      bool dirty;
      Ref::Structure::Digest* digest;
   };

} // namespace Patch

#endif // NOT PatchWidgetH
