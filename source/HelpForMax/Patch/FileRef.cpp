#include "FileRef.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "Package/PackageInfo.h"

File::Ref::Ref(Patch::Structure* structure, const Package::Info* info)
   : Abstract(structure, info)
{
}

void File::Ref::read(const QString& patchName)
{
   structure->clear(); // clear old data

   QFile file(getFilePath(patchName));
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray content = maxFileToDom(file.readAll());
   file.close();

   readContent(content);
}

void File::Ref::write(const QString& patchName)
{
   QFile file(getFilePath(patchName));
   if (!file.open(QIODevice::WriteOnly))
      return;

   QByteArray content = writeContent(patchName);
   content = domToMaxFile(content);

   file.write(content);
   file.close();
}

QString File::Ref::getFilePath(const QString& patchName)
{
   const QString refPath = info->getPath() + "/docs/" + patchName + ".maxref.xml";
   return refPath;
}

void File::Ref::readContent(const QByteArray& content)
{
   QDomDocument doc;
   QDomDocument::ParseResult result = doc.setContent(content);
   if (!result.errorMessage.isEmpty())
   {
      qWarning() << "unable to read xml" << result.errorMessage;
      return;
   }

   const QDomElement rootElement = doc.documentElement();
   readDigest(rootElement, structure->header.digest);

   const QString patcherType = rootElement.attribute("patcher_type", "standard");

   structure->header.patcherType = Patch::Structure::toPatchType(patcherType);

   {
      const QDomElement metaDataElement = rootElement.firstChildElement("metadatalist");
      if (!metaDataElement.isNull())
      {
         const QString& packageName = info->getName();
         for (const QDomElement& element : compileAllDirectChildElements(metaDataElement, "metadata"))
         {
            const QString& name = element.attribute("name");
            if ("tag" != name)
               continue;

            const QString text = readText(element);
            if (packageName != text)
               structure->header.metaTagList.append(text);
         }
      }
   }

   {
      const QDomElement outputListElement = findFirstDirectChildElementWithAttributes(rootElement, "misc", {{"name", "Outputs"}});
      if (!outputListElement.isNull())
      {
         for (const QDomElement& outletElement : compileAllDirectChildElements(outputListElement, "entry"))
         {
            const QString name = outletElement.attribute("name");
            const Patch::Structure::DataType dataType = Patch::Structure::toDataType(name);

            Patch::Structure::Output& output = structure->outputMap[dataType];
            output.active = true;

            readDigest(outletElement, output.digest);
         }
      }
   }

   {
      const QDomElement objArgListElement = rootElement.firstChildElement("objarglist");
      if (!objArgListElement.isNull())
      {
         for (const QDomElement& arguemntElement : compileAllDirectChildElements(objArgListElement, "objarg"))
         {
            Patch::Structure::Argument argument;
            argument.name = arguemntElement.attribute("name");
            argument.optional = ("1" == arguemntElement.attribute("optional"));
            argument.dataType = Patch::Structure::toDataType(arguemntElement.attribute("type"));

            readDigest(arguemntElement, argument.digest);

            structure->argumentList.append(argument);
         }
      }
   }

   {
      const QDomElement attributeListElement = rootElement.firstChildElement("attributelist");
      if (!attributeListElement.isNull())
      {
         for (const QDomElement& attributeElement : compileAllDirectChildElements(attributeListElement, "attribute"))
         {
            const QString name = attributeElement.attribute("name");
            if (!structure->messageNamedMap.contains(name))
            {
               Patch::Structure::AttributesAndMessageNamed attribute;
               attribute.name = name;
               attribute.dataType = Patch::Structure::toDataType(attributeElement.attribute("type"));

               readDigest(attributeElement, attribute.digest);

               structure->messageNamedMap[name] = attribute;
            }

            structure->messageNamedMap[name].patchParts |= Patch::Structure::PatchPart::Attribute;
         }
      }
   }

   {
      const QDomElement messageListElement = rootElement.firstChildElement("methodlist");
      if (!messageListElement.isNull())
      {
         for (const QDomElement& messageElement : compileAllDirectChildElements(messageListElement, "method"))
         {
            const bool isStandard = ("1" == messageElement.attribute("standard"));
            const QString name = messageElement.attribute("name");
            if (isStandard)
            {
               const Patch::Structure::DataType dataType = Patch::Structure::toDataType(name);
               Patch::Structure::MessageTyped& message = structure->messageTypedMap[dataType];

               message.active = true;
               readDigest(messageElement, message.digest);
            }
            else
            {
               Patch::Structure::AttributesAndMessageNamed message;
               if (!structure->messageNamedMap.contains(name))
               {
                  message.name = name;
                  readDigest(messageElement, message.digest);

                  const QDomElement argListElement = messageElement.firstChildElement("arglist");
                  const QDomElement& arguemntElement = argListElement.firstChildElement("arg");
                  message.dataType = Patch::Structure::toDataType(arguemntElement.attribute("type"));

                  structure->messageNamedMap[name] = message;
               }

               structure->messageNamedMap[name].patchParts |= Patch::Structure::PatchPart::MessageNamed;
            }
         }
      }
   }

   {
      const QDomElement seeAlsoListElement = rootElement.firstChildElement("seealsolist");
      if (!seeAlsoListElement.isNull())
      {
         for (QDomElement element = seeAlsoListElement.firstChildElement("seealso"); !element.isNull(); element = element.nextSiblingElement("seealso"))
         {
            const QString& name = element.attribute("name");
            structure->header.seeAlsoList.append(name);
         }
      }
   }
}

QByteArray File::Ref::writeContent(const QString& patchName)
{
   QDomDocument doc;

   QDomElement rootElement = doc.createElement("c74object");
   doc.appendChild(rootElement);
   rootElement.setAttribute("name", patchName);

   rootElement.setAttribute("patcher_type", Patch::Structure::patchTypeName(structure->header.patcherType));
   addDigest(rootElement, structure->header.digest);

   {
      QDomElement metaDataElement = createSubElement(rootElement, "metadatalist");
      createSubElement(metaDataElement, "metadata", info->getAuthor(), {{"name", "author"}});
      createSubElement(metaDataElement, "metadata", info->getName(), {{"name", "tag"}});
      for (const QString& tag : structure->header.metaTagList)
         createSubElement(metaDataElement, "metadata", tag, {{"name", "tag"}});
   }

   {
      QDomElement outputListElement = createSubElement(rootElement, "misc");
      outputListElement.setAttribute("name", "Outputs");
      for (Patch::Structure::Output::Map::const_iterator it = structure->outputMap.constBegin(); it != structure->outputMap.constEnd(); it++)
      {
         const Patch::Structure::Output& output = it.value();
         if (!output.active)
            continue;

         QDomElement outputElement = createSubElement(outputListElement, "entry");
         outputElement.setAttribute("name", Patch::Structure::dataTypeName(output.dataType));

         addDigest(outputElement, it.value().digest);
      }
   }

   {
      QDomElement objArgListElement = createSubElement(rootElement, "objarglist");
      for (const Patch::Structure::Argument& argument : structure->argumentList)
      {
         QDomElement arguemntElement = createSubElement(objArgListElement, "objarg");
         arguemntElement.setAttribute("name", argument.name);
         arguemntElement.setAttribute("optional", argument.optional);
         arguemntElement.setAttribute("type", Patch::Structure::dataTypeName(argument.dataType));

         addDigest(arguemntElement, argument.digest);
      }
   }

   {
      QDomElement attributeListElement = createSubElement(rootElement, "attributelist");
      QDomElement messageListElement = createSubElement(rootElement, "methodlist");

      for (Patch::Structure::AttributesAndMessageNamed::Map::const_iterator it = structure->messageNamedMap.constBegin(); it != structure->messageNamedMap.constEnd(); it++)
      {
         const Patch::Structure::AttributesAndMessageNamed& messageNamed = it.value();
         const QString typeName = Patch::Structure::dataTypeName(messageNamed.dataType);

         if (0 != (messageNamed.patchParts & Patch::Structure::PatchPart::MessageNamed))
         {
            QDomElement messageElement = createSubElement(messageListElement, "method");
            messageElement.setAttribute("name", messageNamed.name);
            messageElement.setAttribute("standard", 0);

            QDomElement argListElement = createSubElement(messageElement, "arglist");

            QDomElement arguemntElement = createSubElement(argListElement, "arg");
            arguemntElement.setAttribute("name", messageNamed.name);
            arguemntElement.setAttribute("optional", 0);
            arguemntElement.setAttribute("type", typeName);

            addDigest(messageElement, messageNamed.digest);
         }
         // no else here !!
         if (0 != (messageNamed.patchParts & Patch::Structure::PatchPart::Attribute))
         {
            QDomElement attributeElement = createSubElement(attributeListElement, "attribute");
            attributeElement.setAttribute("name", messageNamed.name);
            attributeElement.setAttribute("type", typeName);

            addDigest(attributeElement, messageNamed.digest);
         }
      }

      for (Patch::Structure::MessageTyped::Map::const_iterator it = structure->messageTypedMap.constBegin(); it != structure->messageTypedMap.constEnd(); it++)
      {
         const Patch::Structure::MessageTyped& messageTyped = it.value();
         const QString typeName = Patch::Structure::dataTypeName(messageTyped.dataType);

         if (!messageTyped.active)
            continue;

         QDomElement messageElement = createSubElement(messageListElement, "method");
         messageElement.setAttribute("name", typeName);
         messageElement.setAttribute("standard", 1);

         QDomElement argListElement = createSubElement(messageElement, "arglist");

         QDomElement arguemntElement = createSubElement(argListElement, "arg");
         arguemntElement.setAttribute("name", typeName);
         arguemntElement.setAttribute("optional", 0);
         arguemntElement.setAttribute("type", typeName);

         addDigest(messageElement, messageTyped.digest);
      }
   }

   {
      QDomElement seeAlsoListElement = createSubElement(rootElement, "seealsolist");
      for (const QString& seeAlso : structure->header.seeAlsoList)
      {
         createSubElement(seeAlsoListElement, "seealso", QString(), {{"name", seeAlso}});
      }
   }

   QByteArray content;
   content += "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n";
   content += "<?xml-stylesheet href=\"./_c74_ref.xsl\" type=\"text/xsl\"?>\n";
   content += doc.toByteArray(4);

   return content;
}

QDomElement File::Ref::createSubElement(QDomElement parent, const QString& name, const QString& text, const TagMap& tagMap)
{
   QDomElement element = parent.ownerDocument().createElement(name);
   parent.appendChild(element);

   if (!text.isEmpty())
   {
      QDomText textNode = parent.ownerDocument().createTextNode(text);
      element.appendChild(textNode);
   }

   for (TagMap::const_iterator it = tagMap.constBegin(); it != tagMap.constEnd(); it++)
   {
      element.setAttribute(it.key(), it.value());
   }

   return element;
}

void File::Ref::addDigest(const QDomElement& parentElement, const Patch::Structure::Digest& digest)
{
   createSubElement(parentElement, "digest", digest.text);
   if (!digest.description.isEmpty())
   {
      QString description = digest.description;
      description.replace("\n", "&lt;br/&gt;");
      createSubElement(parentElement, "description", description);
   }
}

void File::Ref::readDigest(const QDomElement& parentElement, Patch::Structure::Digest& digest) const
{
   const QDomElement textElement = parentElement.firstChildElement("digest");
   digest.text = readText(textElement);

   const QDomElement descriptionElement = parentElement.firstChildElement("description");
   digest.description = readText(descriptionElement);
}

QString File::Ref::readText(const QDomElement& element) const
{
   if (element.isNull())
      return QString();

   const QDomText textNode = element.firstChild().toText();
   if (textNode.isNull())
      return QString();

   return textNode.data();
}

QDomElement File::Ref::findFirstDirectChildElementWithAttributes(const QDomElement& element, const QString& tag, const TagMap& tagMap) const
{
   for (QDomElement childElement = element.firstChildElement(tag); !childElement.isNull(); childElement = childElement.nextSiblingElement(tag))
   {
      auto hasTags = [&]()
      {
         if (tagMap.empty())
            return true;

         for (TagMap::const_iterator it = tagMap.constBegin(); it != tagMap.constEnd(); it++)
         {
            if (!childElement.hasAttribute(it.key()))
               continue;

            if (it.value() == childElement.attribute(it.key()))
               return true;
         }

         return false;
      };

      if (hasTags())
         return childElement;
   }
   return QDomElement();
}

QList<QDomElement> File::Ref::compileAllDirectChildElements(const QDomElement& element, const QString& tag, const TagMap& tagMap) const
{
   QList<QDomElement> list;
   for (QDomElement childElement = element.firstChildElement(tag); !childElement.isNull(); childElement = childElement.nextSiblingElement(tag))
   {
      auto hasTags = [&]()
      {
         if (tagMap.empty())
            return true;

         for (TagMap::const_iterator it = tagMap.constBegin(); it != tagMap.constEnd(); it++)
         {
            if (!childElement.hasAttribute(it.key()))
               continue;

            if (it.value() == childElement.attribute(it.key()))
               return true;
         }

         return false;
      };

      if (!hasTags())
         continue;

      list.append(childElement);
   }

   return list;
}

QByteArray File::Ref::domToMaxFile(QByteArray domXML) const
{
   domXML.replace("&amp;", "&");
   domXML.replace("&lt;", "<");
   domXML.replace("&gt;", ">");

   return domXML;
}

QByteArray File::Ref::maxFileToDom(QByteArray maxXML) const
{
   for (const QByteArray& tag : Patch::Structure::descriptionMaxTags)
   {
      maxXML.replace("<" + tag + ">", "&lt;" + tag + "&gt;");
      maxXML.replace("</" + tag + ">", "&lt;/" + tag + "&gt;");
   }
   maxXML.replace("<br/>", "\n");

   return maxXML;
}
