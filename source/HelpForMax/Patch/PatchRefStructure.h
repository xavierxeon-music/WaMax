#ifndef PatchRefStructureH
#define PatchRefStructureH

#include <QObject>

#include <QIcon>
#include <QMap>
#include <QString>
#include <QStringList>

namespace Patch
{
   struct Info
   {
      QString name;
      QString folder;
   };

   class RefStructure
   {
      Q_GADGET

   public:
      enum class DataType
      {
         Anything,
         Symbol,
         Float,
         Integer,
         Bang,
         List,
         Signal,
         MultiSignal,
         Dictionary,
         Matrix
      };
      Q_ENUM(DataType)

      enum class PatchType
      {
         Standard,
         Gui,
         Poly,
         Fourier
      };
      Q_ENUM(PatchType)

      enum class PatchPart
      {
         Undefined = 0x00,
         Header = 0x01,
         Argument = 0x02,
         Attribute = 0x04,
         MessageTyped = 0x08,
         MessageNamed = 0x10,
         Output = 0x20
      };
      Q_ENUM(PatchPart)
      Q_DECLARE_FLAGS(PatchParts, PatchPart)

      static const QList<QByteArray> descriptionMaxTags;

      struct Digest
      {
         QString text;
         QString description;
      };

      struct Header
      {
         Digest digest;
         PatchType patcherType = PatchType::Standard;

         QStringList metaTagList;
         QStringList seeAlsoList;
      };

      struct Output
      {
         DataType dataType = DataType::Anything;
         bool active = false;
         Digest digest;

         using List = QList<Output>;
         using Map = QMap<DataType, Output>;
      };

      struct Input
      {
         QString name;
         DataType dataType = DataType::Symbol;
         Digest digest;
      };

      struct MessageTyped : Input
      {
         bool active = false;

         using List = QList<MessageTyped>;
         using Map = QMap<DataType, MessageTyped>;
      };

      // attributes and things in patcherargs with @
      struct AttributesAndMessageNamed : Input
      {
         PatchParts patchParts = PatchPart::Undefined;

         using List = QList<AttributesAndMessageNamed>;
         using Map = QMap<QString, AttributesAndMessageNamed>;
      };

      // things in patcherargs without @
      struct Argument : Input
      {
         bool optional = false;

         using List = QList<Argument>;
      };

      using SeeAlsoList = QStringList;

   public:
      RefStructure();
      virtual ~RefStructure();

   public:
      virtual void clear();
      virtual void setDirty();

      // message type
      static QString dataTypeName(const DataType& type);
      static DataType toDataType(const QString& name);
      static QList<DataType> dataTypeList();

      // patcher type
      static QString patchTypeName(const PatchType& type);
      static PatchType toPatchType(const QString& name);
      static QList<PatchType> patchTypeList();

      // part
      static QString partName(const PatchPart& part);
      static QIcon partIcon(const PatchPart& part);
      static PatchPart toPart(const QString& name);

      void repackNamedMessages();

   public:
      Header header;
      Output::Map outputMap;
      Argument::List argumentList;
      MessageTyped::Map messageTypedMap;
      AttributesAndMessageNamed::Map messageNamedMap;
   };
} // namespace Patch

Q_DECLARE_OPERATORS_FOR_FLAGS(Patch::RefStructure::PatchParts)

#endif // NOT PatchRefStructureH
