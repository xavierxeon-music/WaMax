#include "PatchRefStructure.h"

// structure

const QList<QByteArray> Patch::RefStructure::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};

using DataTypeNameMap = QMap<Patch::RefStructure::DataType, QString>;
static const DataTypeNameMap dataTypeNameMap = {{Patch::RefStructure::DataType::Anything, "anything"}, // must be max names!
                                                {Patch::RefStructure::DataType::Bang, "bang"},
                                                {Patch::RefStructure::DataType::Integer, "int"},
                                                {Patch::RefStructure::DataType::Float, "float"},
                                                {Patch::RefStructure::DataType::Symbol, "symbol"},
                                                {Patch::RefStructure::DataType::List, "list"},
                                                {Patch::RefStructure::DataType::Signal, "signal"},
                                                {Patch::RefStructure::DataType::MultiSignal, "multichannelsignal"},
                                                {Patch::RefStructure::DataType::Dictionary, "dictionary"},
                                                {Patch::RefStructure::DataType::Matrix, "matrix"}};

using PatchTypeNameMap = QMap<Patch::RefStructure::PatchType, QString>;
static const PatchTypeNameMap patchTypeNameMap = {{Patch::RefStructure::PatchType::Standard, "standard"},
                                                  {Patch::RefStructure::PatchType::Gui, "gui"},
                                                  {Patch::RefStructure::PatchType::Poly, "poly"},
                                                  {Patch::RefStructure::PatchType::Fourier, "fourier"}};

using PatchPartNameMap = QMap<Patch::RefStructure::PatchPart, QString>;
static const PatchPartNameMap patchPartNameMap = {{Patch::RefStructure::PatchPart::Undefined, "undefined"},
                                                  {Patch::RefStructure::PatchPart::Header, "Header"},
                                                  {Patch::RefStructure::PatchPart::Argument, "Argument"},
                                                  {Patch::RefStructure::PatchPart::Attribute, "Attribute"},
                                                  {Patch::RefStructure::PatchPart::MessageTyped, "Message Typed"},
                                                  {Patch::RefStructure::PatchPart::MessageNamed, "Message Named"},
                                                  {Patch::RefStructure::PatchPart::Output, "Output"}};

Patch::RefStructure::RefStructure()
   : header()
   , outputMap()
   , argumentList()
   , messageTypedMap()
   , messageNamedMap()
{
   for (const DataType& dataType : dataTypeNameMap.keys())
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

Patch::RefStructure::~RefStructure()
{
}

void Patch::RefStructure::clear()
{
   *this = RefStructure();
}

void Patch::RefStructure::setDirty()
{
}

QString Patch::RefStructure::dataTypeName(const DataType& type)
{
   return dataTypeNameMap.value(type, "anything");
}

Patch::RefStructure::DataType Patch::RefStructure::toDataType(const QString& name)
{
   for (DataTypeNameMap ::const_iterator it = dataTypeNameMap.constBegin(); it != dataTypeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return DataType::Anything;
}

QList<Patch::RefStructure::DataType> Patch::RefStructure::dataTypeList()
{
   return dataTypeNameMap.keys();
}

QString Patch::RefStructure::patchTypeName(const PatchType& type)
{
   return patchTypeNameMap.value(type, "standard");
}

Patch::RefStructure::PatchType Patch::RefStructure::toPatchType(const QString& name)
{
   for (PatchTypeNameMap::const_iterator it = patchTypeNameMap.constBegin(); it != patchTypeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchType::Standard;
   ;
}

QList<Patch::RefStructure::PatchType> Patch::RefStructure::patchTypeList()
{
   return patchTypeNameMap.keys();
}

QString Patch::RefStructure::partName(const PatchPart& part)
{
   return patchPartNameMap.value(part, "undefined");
}

QIcon Patch::RefStructure::partIcon(const PatchPart& part)
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

   return QIcon();
}

Patch::RefStructure::PatchPart Patch::RefStructure::toPart(const QString& name)
{
   for (PatchPartNameMap ::const_iterator it = patchPartNameMap.constBegin(); it != patchPartNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchPart::Undefined;
   ;
}

void Patch::RefStructure::repackNamedMessages()
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
