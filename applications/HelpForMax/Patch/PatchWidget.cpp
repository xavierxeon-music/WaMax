#include "PatchWidget.h"

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QScrollArea>

#include "DelegateDataType.h"
#include "DelegatePatchType.h"
#include "DescriptionHighlighter.h"
#include "FileHelp.h"
#include "FileInit.h"
#include "FileRef.h"
#include "PackageInfo.h"
#include "PatchModelArgument.h"
#include "PatchModelHeader.h"
#include "PatchModelNamedMessage.h"
#include "PatchModelOutput.h"
#include "PatchModelTypedMessage.h"
#include "SuggestModelArgument.h"
#include "SuggestModelNamedMessage.h"
#include "SuggestModelOutput.h"
#include "SuggestModelTypedMessage.h"

Patch::Widget::Widget(TabWidget* tabWidget, const Package::Info* packageInfo, const QString& patchFileName)
   : QSplitter(tabWidget)
   , Max::Patcher()
   , RefStructure()
   , tabWidget(tabWidget)
   , structureWidget(nullptr)
   , suggestionWidgets()
   , packageInfo(packageInfo)
   , path(patchFileName)
   , patchInfo{}
   , modelList()
   , digest(nullptr)
{
   // left: scroll area
   QScrollArea* scrollArea = new QScrollArea(this);
   {
      QWidget* content = new QWidget();
      Ui::PatchWidget::setupUi(content);

      scrollArea->setFrameShadow(QFrame::Plain);
      scrollArea->setFrameShape(QFrame::NoFrame);
      scrollArea->setWidgetResizable(true);
      scrollArea->setWidget(content);

      setIcon(patchIcon, RefStructure::PatchPart::Header);
      setIcon(argumentIcon, RefStructure::PatchPart::Argument);
      setIcon(typedMessageIcon, RefStructure::PatchPart::MessageTyped);
      setIcon(attributeIcon, RefStructure::PatchPart::Attribute);
      setIcon(nameMessageIcon, RefStructure::PatchPart::MessageNamed);
      setIcon(outputIcon, RefStructure::PatchPart::Output);
      setIcon(otherIcon, RefStructure::PatchPart::Other);

      structureWidget = new Structure::Widget(this);
      RefStructure* suggestStructure = nullptr;

      // set models
      Suggest::Model::Argument* argumentSuggestModel = new Suggest::Model::Argument(this, suggestStructure);
      argumentSuggestTree->init(this, argumentSuggestModel);
      argumentSuggestTree->setButton(argumnetTransferButton);
      argumentSuggestTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentSuggestModel));

      suggestionWidgets << argumentSuggestTree << argumnetTransferButton;

      Suggest::Model::TypedMessage* typedMessageSuggestModel = new Suggest::Model::TypedMessage(this, suggestStructure);
      typedMessageSuggestTree->init(this, typedMessageSuggestModel);
      typedMessageSuggestTree->setButton(typedMessageTransferButton);
      suggestionWidgets << typedMessageSuggestTree << typedMessageTransferButton;

      Suggest::Model::NamedMessage* namedMessageSuggestModel = new Suggest::Model::NamedMessage(this, suggestStructure);
      namedMessageSuggestTree->init(this, namedMessageSuggestModel);
      namedMessageSuggestTree->setButton(namedMessageTransferButton);
      suggestionWidgets << namedMessageSuggestTree << namedMessageTransferButton;

      Suggest::Model::Output* outputSuggestModel = new Suggest::Model::Output(this, suggestStructure);
      outputSuggestTree->init(this, outputSuggestModel);
      outputSuggestTree->setButton(outputTransferButton);
      suggestionWidgets << outputSuggestTree << outputTransferButton;

      // set models
      Model::Header* headerPatchModel = new Model::Header(this, this);
      modelList.append(headerPatchModel);
      headerTree->init(this, headerPatchModel, 1);
      headerTree->setItemDelegateForColumn(0, new Delegate::PatchType(this, headerPatchModel));

      Model::Argument* argumentPatchModel = new Model::Argument(this, this);
      modelList.append(argumentPatchModel);
      argumentTree->init(this, argumentPatchModel);
      argumentTree->setButtons(argumentAddButton, argumentRemoveButton);
      argumentTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentPatchModel));

      Model::TypedMessage* typedMessagPatcheModel = new Model::TypedMessage(this, this);
      modelList.append(typedMessagPatcheModel);
      typedMessageTree->init(this, typedMessagPatcheModel);

      Model::NamedMessage* namedMessagePatchModel = new Model::NamedMessage(this, this);
      modelList.append(namedMessagePatchModel);
      namedMessageTree->init(this, namedMessagePatchModel);
      namedMessageTree->setButtons(namedMessageAddButton, namedMessageRemoveButton);
      namedMessageTree->setItemDelegateForColumn(1, new Delegate::DataType(this, namedMessagePatchModel));

      Model::Output* outputPatchModel = new Model::Output(this, this);
      modelList.append(outputPatchModel);
      outputTree->init(this, outputPatchModel);
      outputTree->setButtons(outputAddButton, outputRemoveButton);
   }

   // right: digest area
   QWidget* editArea = new QWidget(this);
   {
      Ui::DigestWidget::setupUi(editArea);

      new DescriptionHighlighter(descriptionEdit->document());
      descriptionIcon->setPixmap(QIcon(":/DocDescription.svg").pixmap(16, 16));

      connect(digestEdit, &QLineEdit::editingFinished, this, &Widget::slotSaveDigestText);
      connect(descriptionEdit, &QTextEdit::textChanged, this, &Widget::slotSaveDigestDescription);
   }

   addWidget(scrollArea);
   addWidget(editArea);
   addWidget(structureWidget);

   // load content
   patchInfo = packageInfo->extractPatchInfo(path);
   propagateDirty(false);

   readPatch(path);
   structureWidget->load(this);

   File::Ref(packageInfo, this).read(patchInfo);
   rebuild();
}

Patch::Widget::~Widget()
{
   tabWidget->emitSignalCheckDirty();
}

const QString& Patch::Widget::getPath() const
{
   return path;
}

const Package::Info* Patch::Widget::getPacakgeInfo() const
{
   return packageInfo;
}

const Patch::Info& Patch::Widget::getPatchInfo() const
{
   return patchInfo;
}

void Patch::Widget::writeRef()
{
   File::Ref(packageInfo, this).write(patchInfo);
   File::Help(packageInfo).write(patchInfo);
   File::Init(packageInfo, this).write(patchInfo);
   propagateDirty(false);
}

void Patch::Widget::openInMax()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Patch::Widget::openXML()
{
   const QString refPath = File::Ref(packageInfo, this).getFilePath(patchInfo);
   QDesktopServices::openUrl(QUrl::fromLocalFile(refPath));
}

bool Patch::Widget::isDirty() const
{
   return dirty;
}

void Patch::Widget::setToolsVisible(TabWidget::ToolsVisible toolsVisible)
{
   const bool showSuggestions = TabWidget::ToolVisibility::Suggestions & toolsVisible;
   for (QWidget* widget : suggestionWidgets)
      widget->setVisible(showSuggestions);

   structureWidget->setVisible(TabWidget::ToolVisibility::Structure & toolsVisible);
}

void Patch::Widget::slotSetPatchDigest()
{
   setDigest(&header.digest, RefStructure::PatchPart::Header);
}

void Patch::Widget::slotSaveDigestText()
{
   digest->text = digestEdit->text();
   setDirty();
}

void Patch::Widget::slotSaveDigestDescription()
{
   digest->description = descriptionEdit->toPlainText();
   setDirty();
}

void Patch::Widget::setDigest(Digest* newDigest, const PatchPart& part)
{
   digest = newDigest;
   if (!digest)
   {
      topicInfo->clear();
      digestEdit->clear();
      descriptionEdit->clear();
      return;
   }

   setIcon(topicIcon, part);

   topicInfo->setText(partName(part));
   digestEdit->setText(digest->text);

   descriptionEdit->blockSignals(true);
   descriptionEdit->setText(digest->description);
   descriptionEdit->blockSignals(false);
}

void Patch::Widget::rebuild()
{
   for (Model::Abstract* model : modelList)
      model->rebuild();

   setDigest(&header.digest, RefStructure::PatchPart::Header);
}

void Patch::Widget::update()
{
   for (Model::Abstract* model : modelList)
      model->update();
}

void Patch::Widget::setDirty()
{
   propagateDirty(true);
   update();
}

void Patch::Widget::propagateDirty(bool isDirty)
{
   dirty = isDirty;
   tabWidget->emitSignalCheckDirty();
}

void Patch::Widget::setIcon(QLabel* iconLabel, RefStructure::PatchPart part)
{
   iconLabel->setPixmap(partIcon(part).pixmap(16, 16));
}
