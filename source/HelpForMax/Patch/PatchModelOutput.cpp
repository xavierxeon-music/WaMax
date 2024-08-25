#include "PatchModelOutput.h"

Patch::Model::Output::Output(QObject* parent, Structure* structure)
   : Abstract(parent, structure, Structure::PatchPart::Output)
{
}

void Patch::Model::Output::update()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* typeItem = invisibleRootItem()->child(row, 0);
      QStandardItem* activeItem = invisibleRootItem()->child(row, 1);
      QStandardItem* digestItem = invisibleRootItem()->child(row, 2);

      const Structure::DataType type = typeItem->data().value<Structure::DataType>();
      const Structure::Output& output = structure->outputMap[type];

      activeItem->setCheckState(output.active ? Qt::Checked : Qt::Unchecked);

      updateDigestItem(digestItem, output.digest);
   }

   emit signalDataEdited();
}

void Patch::Model::Output::rebuild()
{
   beginResetModel();
   clear();
   setHorizontalHeaderLabels({"Type", "Active", "Digest"});

   for (const Structure::DataType& type : structure->dataTypeList())
   {
      QStandardItem* typeItem = new QStandardItem(Structure::dataTypeName(type));
      typeItem->setEditable(false);
      typeItem->setData(QVariant::fromValue(type));

      QStandardItem* activeItem = new QStandardItem();
      activeItem->setEditable(false);
      activeItem->setCheckable(true);
      activeItem->setIcon(QIcon(":/DocOutput.svg"));

      QStandardItem* digestItem = new QStandardItem();
      digestItem->setEditable(false);

      invisibleRootItem()->appendRow({typeItem, activeItem, digestItem});
   }

   endResetModel();
   update();
}

Patch::Structure::Digest* Patch::Model::Output::getDigest(const QModelIndex& index)
{
   QStandardItem* typeItem = invisibleRootItem()->child(index.row(), 0);
   const Structure::DataType type = typeItem->data().value<Structure::DataType>();

   Structure::Output& output = structure->outputMap[type];
   return &(output.digest);
}

bool Patch::Model::Output::setData(const QModelIndex& index, const QVariant& value, int role)
{
   const bool result = QStandardItemModel::setData(index, value, role);

   QStandardItem* typeItem = invisibleRootItem()->child(index.row(), 0);
   const Structure::DataType type = typeItem->data().value<Structure::DataType>();

   Structure::Output& output = structure->outputMap[type];

   if (Qt::CheckStateRole == role)
   {
      const bool enabled = (Qt::Checked == value.toInt());

      if (1 == index.column())
      {
         output.active = enabled;
         structure->setDirty();
      }
   }

   emit signalDataEdited();
   return result;
}
