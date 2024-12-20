#ifndef SuggestTreeViewH
#define SuggestTreeViewH

#include <QTreeView>

namespace Suggest
{
   class TreeView : public QTreeView
   {
      Q_OBJECT

   public:
      TreeView(QWidget* parent);
   };
} // namespace Suggest

#endif // NOT SuggestTreeViewH
