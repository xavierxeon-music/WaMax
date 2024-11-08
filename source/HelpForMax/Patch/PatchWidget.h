#ifndef PatchWidgetH
#define PatchWidgetH

#include "../_Forms/ui_DigestWidget.h"
#include "../_Forms/ui_PatchWidget.h"
#include "PatchRefStructure.h"
#include <QWidget>

#include "PatchModelAbstract.h"

namespace Package
{
   class Info;
}

namespace Patch
{
   class TabWidget;

   class Widget : public QWidget, private RefStructure, private Ui::PatchWidget, private Ui::DigestWidget
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

   private:
      friend class TreeView;

   private slots:
      void slotSetPatchDigest();
      void slotSaveDigestText();
      void slotSaveDigestDescription();

   private:
      void setDigest(Digest* newDigest, const PatchPart& part);
      void rebuild();
      void update();
      void setDirty() override;
      void propagateDirty(bool isDirty);
      void setIcon(QLabel* iconLabel, RefStructure::PatchPart part);

   private:
      TabWidget* tabWidget;

      const Package::Info* packageInfo;
      QString path;
      Patch::Info patchInfo;

      Model::Abstract::List modelList;
      bool dirty;
      Digest* digest;
   };

} // namespace Patch

#endif // NOT PatchWidgetH
