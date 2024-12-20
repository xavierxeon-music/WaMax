#ifndef PatchSuggestViewH
#define PatchSuggestViewH

#include <QToolButton>
#include <QTreeView>

namespace Patch
{
   namespace Model
   {
      class Abstract;
   }

   class Widget;

   class SuggestView : public QTreeView
   {
      Q_OBJECT

   public:
      SuggestView(QWidget* parent);

   public:
      void init(Widget* widget, Model::Abstract* model);
      void setButton(QToolButton* transferButton);

   private slots:
      void slotResizeColumns();
      void slotTransfer();

   private:
      Widget* widget;
      Model::Abstract* model;
   };
} // namespace Patch

#endif // NOT PatchSuggestViewH
