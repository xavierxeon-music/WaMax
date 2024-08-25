#include "PatchStructure.h"

// structure

const QList<QByteArray> Patch::Structure::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};

using DataTypeNameMap = QMap<Patch::Structure::DataType, QString>;
static const DataTypeNameMap dataTypeNameMap = {{Patch::Structure::DataType::Anything, "anything"}, // must be max names!
                                                {Patch::Structure::DataType::Bang, "bang"},
                                                {Patch::Structure::DataType::Integer, "int"},
                                                {Patch::Structure::DataType::Float, "float"},
                                                {Patch::Structure::DataType::Symbol, "symbol"},
                                                {Patch::Structure::DataType::List, "list"},
                                                {Patch::Structure::DataType::Signal, "signal"},
                                                {Patch::Structure::DataType::MultiSignal, "multichannelsignal"},
                                                {Patch::Structure::DataType::Dictionary, "dictionary"},
                                                {Patch::Structure::DataType::Matrix, "matrix"}};

using PatchTypeNameMap = QMap<Patch::Structure::PatchType, QString>;
static const PatchTypeNameMap patchTypeNameMap = {{Patch::Structure::PatchType::Standard, "standard"},
                                                  {Patch::Structure::PatchType::Gui, "gui"},
                                                  {Patch::Structure::PatchType::Poly, "poly"},
                                                  {Patch::Structure::PatchType::Fourier, "fourier"}};

using PatchPartNameMap = QMap<Patch::Structure::PatchPart, QString>;
static const PatchPartNameMap patchPartNameMap = {{Patch::Structure::PatchPart::Undefined, "undefined"},
                                                  {Patch::Structure::PatchPart::Header, "Header"},
                                                  {Patch::Structure::PatchPart::Argument, "Argument"},
                                                  {Patch::Structure::PatchPart::Attribute, "Attribute"},
                                                  {Patch::Structure::PatchPart::MessageTyped, "Message Typed"},
                                                  {Patch::Structure::PatchPart::MessageNamed, "Message Named"},
                                                  {Patch::Structure::PatchPart::Output, "Output"}};

Patch::Structure::Structure()
   : header()
   , outputMap()
   , argumentList()
   , messageTypedMap()
   , messageNamedMap()
{
   for (const DataType& dataType : dataTypeNameMap.keys())
   {
      {
         Structure::MessageTyped message;
         message.active = false;
         message.dataType = dataType;

         messageTypedMap[dataType] = message;
      }
      {
         Structure::Output output;
         output.active = false;
         output.dataType = dataType;

         outputMap[dataType] = output;
      }
   }
}

Patch::Structure::~Structure()
{
}

void Patch::Structure::clear()
{
   *this = Structure();
}

void Patch::Structure::setDirty()
{
}

QString Patch::Structure::dataTypeName(const DataType& type)
{
   return dataTypeNameMap.value(type, "anything");
}

Patch::Structure::DataType Patch::Structure::toDataType(const QString& name)
{
   for (DataTypeNameMap ::const_iterator it = dataTypeNameMap.constBegin(); it != dataTypeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return DataType::Anything;
}

QList<Patch::Structure::DataType> Patch::Structure::dataTypeList()
{
   return dataTypeNameMap.keys();
}

QString Patch::Structure::patchTypeName(const PatchType& type)
{
   return patchTypeNameMap.value(type, "standard");
}

Patch::Structure::PatchType Patch::Structure::toPatchType(const QString& name)
{
   for (PatchTypeNameMap::const_iterator it = patchTypeNameMap.constBegin(); it != patchTypeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchType::Standard;
   ;
}

QList<Patch::Structure::PatchType> Patch::Structure::patchTypeList()
{
   return patchTypeNameMap.keys();
}

QString Patch::Structure::partName(const PatchPart& part)
{
   return patchPartNameMap.value(part, "undefined");
}

QIcon Patch::Structure::partIcon(const PatchPart& part)
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

Patch::Structure::PatchPart Patch::Structure::toPart(const QString& name)
{
   for (PatchPartNameMap ::const_iterator it = patchPartNameMap.constBegin(); it != patchPartNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchPart::Undefined;
   ;
}

void Patch::Structure::repackNamedMessages()
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
