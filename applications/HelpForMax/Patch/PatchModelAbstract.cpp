#include "PatchModelAbstract.h"

QList<Patch::Model::Abstract*> Patch::Model::Abstract::instanceList;

Patch::Model::Abstract::Abstract(QObject* parent, Max::RefStructure& structure, const Max::RefStructure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , part(part)
{
   instanceList.append(this);
}

Patch::Model::Abstract::~Abstract()
{
   instanceList.removeAll(this);
}

void Patch::Model::Abstract::updateDigestItem(QStandardItem* digestItem, const Max::RefStructure::Digest& digest)
{
   QString description = digest.text;
   digestItem->setText(description);

   if (digest.description.isEmpty())
      digestItem->setIcon(QIcon());
   else
      digestItem->setIcon(QIcon(":/DocDescription.svg"));
}

void Patch::Model::Abstract::createBeforeItem(const QModelIndex& index)
{
   Q_UNUSED(index)
}

void Patch::Model::Abstract::removeItem(const QModelIndex& index)
{
   Q_UNUSED(index)
}

const Max::RefStructure::PatchPart& Patch::Model::Abstract::getPart() const
{
   return part;
}

void Patch::Model::Abstract::updateAll()
{
   for (Abstract* model : instanceList)
      model->update();
}

void Patch::Model::Abstract::rebuildAll()
{
   for (Abstract* model : instanceList)
      model->rebuild();
}
