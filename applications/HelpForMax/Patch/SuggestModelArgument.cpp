#include "SuggestModelArgument.h"

Suggest::Model::Argument::Argument(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::Argument)
   , Delegate::DataType::Source()
{
}

void Suggest::Model::Argument::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Name", "Type"});

   while (0 < invisibleRootItem()->rowCount())
      invisibleRootItem()->removeRow(0);

   for (int row = 0; row < structure.argumentList.count(); row++)
   {
      const Ref::Structure::Argument& argument = structure.argumentList.at(row);

      QStandardItem* nameItem = new QStandardItem();
      nameItem->setEditable(false);
      nameItem->setText(argument.name);

      QStandardItem* typeItem = new QStandardItem();
      nameItem->setEditable(false);
      typeItem->setText(Max::dataTypeName(argument.dataType));

      invisibleRootItem()->appendRow({nameItem, typeItem});
   }

   endResetModel();
}

Max::DataType Suggest::Model::Argument::getDataType(const int index)
{
   const Ref::Structure::Argument& argument = structure.argumentList.at(index);

   return argument.dataType;
}
