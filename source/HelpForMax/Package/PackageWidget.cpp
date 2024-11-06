#include "PackageWidget.h"

#include "PackageModel.h"

Package::Widget::Widget(QWidget* parent, const Info* info)
   : QWidget(parent)
   , info(info)
   , model(nullptr)
{
   setupUi(this);

   packageNameInfo->setText("<b>...</b>");

   model = new Model(this, info);
   model->create();
   packageTree->setModel(model);

   connect(packageTree, &QTreeView::doubleClicked, this, &Widget::slotItemDoubleClicked);
}

const Package::Info* Package::Widget::getPackageInfo() const
{
   return info;
}

void Package::Widget::selectPatch(const QString& patchFileName)
{
   QModelIndex index = model->find(patchFileName);
   if (!index.isValid())
      return;

   packageTree->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
}

void Package::Widget::checkUpToDate()
{
   model->update();
}

void Package::Widget::slotItemDoubleClicked(const QModelIndex& index)
{
   QStandardItem* item = model->itemFromIndex(index);
   const QString path = item->data().toString();
   if (path.isEmpty())
      return;

   emit signalPatchSeleted(path, info);
}
