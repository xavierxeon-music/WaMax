#ifndef PackageViewH
#define PackageViewH

#include "ui_PackageWidget.h"
#include <QWidget>

namespace Package
{
   class Model;
   class Info;

   class Widget : public QWidget, private Ui::Widget
   {
      Q_OBJECT

   public:
      Widget(QWidget* parent, const Info* info);

   signals:
      void signalPatchSeleted(const QString& path, const Info* info);

   public:
      const Info* getPackageInfo() const;
      void selectPatch(const QString& patchFileName);

   private slots:
      void slotItemDoubleClicked(const QModelIndex& index);

   private:
      const Info* info;
      Model* model;
   };
} // namespace Package

#endif // NOT PackageViewH
