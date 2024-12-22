#include "MaxRefStructure.h"

// structure

const QList<QByteArray> Max::RefStructure::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};

using PatchTypeNameMap = QMap<Max::RefStructure::PatchType, QString>;
static const PatchTypeNameMap patchTypeNameMap = {{Max::RefStructure::PatchType::Standard, "standard"},
                                                  {Max::RefStructure::PatchType::Gui, "gui"},
                                                  {Max::RefStructure::PatchType::Poly, "poly"},
                                                  {Max::RefStructure::PatchType::Fourier, "fourier"}};

using PatchPartNameMap = QMap<Max::RefStructure::PatchPart, QString>;
static const PatchPartNameMap patchPartNameMap = {{Max::RefStructure::PatchPart::Undefined, "undefined"},
                                                  {Max::RefStructure::PatchPart::Header, "Header"},
                                                  {Max::RefStructure::PatchPart::Argument, "Argument"},
                                                  {Max::RefStructure::PatchPart::Attribute, "Attribute"},
                                                  {Max::RefStructure::PatchPart::MessageTyped, "Message Typed"},
                                                  {Max::RefStructure::PatchPart::MessageNamed, "Message Named"},
                                                  {Max::RefStructure::PatchPart::Output, "Output"}};

Max::RefStructure::RefStructure()
   : header()
   , outputMap()
   , argumentList()
   , messageTypedMap()
   , messageNamedMap()
   , clearHook()
   , dirtyHook()
{
   for (const Max::DataType& dataType : Max::dataTypeList())
   {
      {
         RefStructure::MessageTyped message;
         message.active = false;
         message.dataType = dataType;

         messageTypedMap[dataType] = message;
      }
      {
         RefStructure::Output output;
         output.active = false;
         output.dataType = dataType;

         outputMap[dataType] = output;
      }
   }
}

Max::RefStructure::~RefStructure()
{
}

void Max::RefStructure::clear()
{
   *this = RefStructure();

   if (clearHook)
      clearHook();
}

void Max::RefStructure::setDirty()
{
   if (dirtyHook)
      dirtyHook();
}

QString Max::RefStructure::patchTypeName(const PatchType& type)
{
   return patchTypeNameMap.value(type, "standard");
}

Max::RefStructure::PatchType Max::RefStructure::toPatchType(const QString& name)
{
   for (PatchTypeNameMap::const_iterator it = patchTypeNameMap.constBegin(); it != patchTypeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchType::Standard;
   ;
}

QList<Max::RefStructure::PatchType> Max::RefStructure::patchTypeList()
{
   return patchTypeNameMap.keys();
}

QString Max::RefStructure::partName(const PatchPart& part)
{
   return patchPartNameMap.value(part, "undefined");
}

QIcon Max::RefStructure::partIcon(const PatchPart& part)
{
   if (PatchPart::Header == part)
      return QIcon(":/PatchGeneral.svg");
   else if (PatchPart::Argument == part)
      return QIcon(":/DocArgument.svg");
   else if (PatchPart::Attribute == part)
      return QIcon(":/DocAttribute.svg");
   else if (PatchPart::MessageTyped == part)
      return QIcon(":/DocMessageTyped.svg");
   else if (PatchPart::MessageNamed == part)
      return QIcon(":/DocMessageNamed.svg");
   else if (PatchPart::Output == part)
      return QIcon(":/DocOutput.svg");
   else if (PatchPart::Other == part)
      return QIcon(":/OverviewGeneral.svg");
   return QIcon();
}

Max::RefStructure::PatchPart Max::RefStructure::toPart(const QString& name)
{
   for (PatchPartNameMap ::const_iterator it = patchPartNameMap.constBegin(); it != patchPartNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchPart::Undefined;
   ;
}

void Max::RefStructure::repackNamedMessages()
{
   AttributesAndMessageNamed::List list = messageNamedMap.values();
   messageNamedMap.clear();

   for (const AttributesAndMessageNamed& message : list)
   {
      QString name = message.name;
      // avoid duplicate keys
      while (messageNamedMap.contains(name))
         name += "_";

      messageNamedMap.insert(name, message);
   }
}
