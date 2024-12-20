#include "PatchSuggestView.h"

#include <QTimer>

#include "PatchModelAbstract.h"
#include "PatchWidget.h"

Patch::SuggestView::SuggestView(QWidget* parent)
   : QTreeView(parent)
   , widget(nullptr)
   , model(nullptr)
{
   setRootIsDecorated(false);
   setUniformRowHeights(true);
}

void Patch::SuggestView::init(Widget* widget, Model::Abstract* model)
{
   this->widget = widget;
   this->model = model;

   setModel(model);

   connect(model, &Model::Abstract::signalDataEdited, this, &SuggestView::slotResizeColumns);
}

void Patch::SuggestView::setButton(QToolButton* transferButton)
{
   static const QString styleSheet = "QToolButton { border: 0px none #8f8f91;}";

   static const QString transfer = QString::fromUtf8("\u27a4");
   transferButton->setStyleSheet(styleSheet);
   transferButton->setText(transfer);

   connect(transferButton, &QAbstractButton::clicked, this, &SuggestView::slotTransfer);
}

void Patch::SuggestView::slotResizeColumns()
{
   auto resizeIternal = [this]()
   {
      for (int col = 0; col < model->columnCount(); col++)
         this->resizeColumnToContents(col);

      QModelIndex index = this->model->index(0, 0);
      const int height = this->header()->height() + this->rowHeight(index);

      this->setMaximumHeight(height + 10);
   };

   QTimer::singleShot(10, this, resizeIternal);
}

void Patch::SuggestView::slotTransfer()
{
}
