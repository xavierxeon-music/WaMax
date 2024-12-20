#ifndef SuggestTreeViewH
#define SuggestTreeViewH

#include <QTreeView>

#include <QToolButton>

namespace Patch
{
   class Widget;
} // namespace Patch

namespace Suggest
{
   namespace Model
   {
      class Abstract;
   }

   class TreeView : public QTreeView
   {
      Q_OBJECT

   public:
      TreeView(QWidget* parent);

   public:
      void init(Patch::Widget* widget, Model::Abstract* model);
      void setButton(QToolButton* transferButton);

   private slots:
      void slotResizeColumns();
      void slotTransfer();

   private:
      Patch::Widget* widget;
      Model::Abstract* model;
   };
} // namespace Suggest

#endif // NOT SuggestTreeViewH
