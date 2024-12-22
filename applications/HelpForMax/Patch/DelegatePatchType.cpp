#include "DelegatePatchType.h"

#include <QComboBox>

Delegate::PatchType::PatchType(QObject* parent, PatchType::Source* source)
   : QStyledItemDelegate(parent)
   , source(source)
{
}

QWidget* Delegate::PatchType::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QComboBox* comboBox = new QComboBox(parent);
   comboBox->setFrame(false);
   for (const Max::RefStructure::PatchType& type : Max::RefStructure::patchTypeList())
   {
      comboBox->addItem(Max::RefStructure::patchTypeName(type));
   }

   for (int index = 0; index < comboBox->count(); index++)
   {
      comboBox->setItemData(index, Qt::AlignCenter, Qt::TextAlignmentRole);
   }

   return comboBox;
}

void Delegate::PatchType::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

   const Max::RefStructure::PatchType type = source->getPatchType(index.row());
   const int typeIndex = comboBox->findText(Max::RefStructure::patchTypeName(type));
   comboBox->setCurrentIndex(typeIndex);
}

void Delegate::PatchType::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

   const QString typeText = comboBox->currentText();
   model->setData(index, typeText, Qt::EditRole);
}

void Delegate::PatchType::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
