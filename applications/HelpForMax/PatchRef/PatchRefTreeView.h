#ifndef PatchTreeViewH
#define PatchTreeViewH

#include <QTreeView>

#include <QToolButton>

namespace PatchRef
{
   namespace Model
   {
      class Abstract;
   }
} // namespace PatchRef

namespace Patch
{

   class Widget;

   class TreeView : public QTreeView
   {
      Q_OBJECT

   public:
      TreeView(QWidget* parent);

   public:
      void init(Widget* widget, PatchRef::Model::Abstract* model, int forceRowHeight = -1);
      void setButtons(QToolButton* addButton, QToolButton* removeButton);

   private slots:
      void slotResizeColumns();
      void slotUpdateDigest(const QModelIndex& index);
      void slotAddElement();
      void slotRemoveElement();

   private:
      Widget* widget;
      PatchRef::Model::Abstract* model;

      int forceRowHeight;
   };
} // namespace Patch

#endif // NOT PatchTreeViewH
