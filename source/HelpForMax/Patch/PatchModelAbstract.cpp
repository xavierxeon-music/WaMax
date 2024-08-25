#include "PatchModelAbstract.h"

Patch::Model::Abstract::Abstract(QObject* parent, Structure* structure, const Structure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , part(part)
{
}

void Patch::Model::Abstract::updateDigestItem(QStandardItem* digestItem, const Structure::Digest& digest)
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

const Patch::Structure::PatchPart& Patch::Model::Abstract::getPart() const
{
   return part;
}
