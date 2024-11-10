#ifndef PatchSuggestViewH
#define PatchSuggestViewH

#include <QTreeView>

namespace Patch
{
   class SuggestView : public QTreeView
   {
      Q_OBJECT

   public:
      SuggestView(QWidget* parent);
   };
} // namespace Patch

#endif // NOT PatchSuggestViewH
