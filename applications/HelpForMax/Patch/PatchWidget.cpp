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
   , maxRef()
   , maxPatch()
   , tabWidget(tabWidget)
   , structureWidget(nullptr)
   , packageInfo(packageInfo)
   , path(patchFileName)
   , patchInfo{}
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

      setIcon(patchIcon, Max::RefStructure::PatchPart::Header);
      setIcon(argumentIcon, Max::RefStructure::PatchPart::Argument);
      setIcon(typedMessageIcon, Max::RefStructure::PatchPart::MessageTyped);
      setIcon(attributeIcon, Max::RefStructure::PatchPart::Attribute);
      setIcon(nameMessageIcon, Max::RefStructure::PatchPart::MessageNamed);
      setIcon(outputIcon, Max::RefStructure::PatchPart::Output);
      setIcon(otherIcon, Max::RefStructure::PatchPart::Other);

      // set models
      Suggest::Model::Argument* argumentSuggestModel = new Suggest::Model::Argument(this, maxPatch);
      argumentSuggestTree->init(this, argumentSuggestModel);
      argumentSuggestTree->setButton(argumnetTransferButton);
      argumentSuggestTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentSuggestModel));

      Suggest::Model::TypedMessage* typedMessageSuggestModel = new Suggest::Model::TypedMessage(this, maxPatch);
      typedMessageSuggestTree->init(this, typedMessageSuggestModel);
      typedMessageSuggestTree->setButton(typedMessageTransferButton);

      Suggest::Model::NamedMessage* namedMessageSuggestModel = new Suggest::Model::NamedMessage(this, maxPatch);
      namedMessageSuggestTree->init(this, namedMessageSuggestModel);
      namedMessageSuggestTree->setButton(namedMessageTransferButton);

      Suggest::Model::Output* outputSuggestModel = new Suggest::Model::Output(this, maxPatch);
      outputSuggestTree->init(this, outputSuggestModel);
      outputSuggestTree->setButton(outputTransferButton);

      // set models
      Model::Header* headerPatchModel = new Model::Header(this, maxRef);
      headerPatchTree->init(this, headerPatchModel, 1);
      headerPatchTree->setItemDelegateForColumn(0, new Delegate::PatchType(this, headerPatchModel));

      Model::Argument* argumentPatchModel = new Model::Argument(this, maxRef);
      argumentPatchTree->init(this, argumentPatchModel);
      argumentPatchTree->setButtons(argumentAddButton, argumentRemoveButton);
      argumentPatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentPatchModel));

      Model::TypedMessage* typedMessagPatcheModel = new Model::TypedMessage(this, maxRef);
      typedMessagePatchTree->init(this, typedMessagPatcheModel);

      Model::NamedMessage* namedMessagePatchModel = new Model::NamedMessage(this, maxRef);
      namedMessagePatchTree->init(this, namedMessagePatchModel);
      namedMessagePatchTree->setButtons(namedMessageAddButton, namedMessageRemoveButton);
      namedMessagePatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, namedMessagePatchModel));

      Model::Output* outputPatchModel = new Model::Output(this, maxRef);
      outputPatchTree->init(this, outputPatchModel);
      outputPatchTree->setButtons(outputAddButton, outputRemoveButton);
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

   structureWidget = new Structure::Widget(this);

   addWidget(scrollArea);
   addWidget(editArea);
   addWidget(structureWidget);

   // load content
   patchInfo = packageInfo->extractPatchInfo(path);
   propagateDirty(false);

   maxPatch.readPatch(path);
   structureWidget->load(maxPatch);

   maxRef.dirtyHook = std::bind(&Widget::setDirty, this);

   File::Ref(packageInfo, maxRef).read(patchInfo);
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
   File::Ref(packageInfo, maxRef).write(patchInfo);
   File::Help(packageInfo).write(patchInfo);
   File::Init(packageInfo, maxRef).write(patchInfo);
   propagateDirty(false);
}

void Patch::Widget::openInMax()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Patch::Widget::openXML()
{
   const QString refPath = File::Ref(packageInfo, maxRef).getFilePath(patchInfo);
   QDesktopServices::openUrl(QUrl::fromLocalFile(refPath));
}

bool Patch::Widget::isDirty() const
{
   return dirty;
}

void Patch::Widget::setToolsVisible(TabWidget::ToolsVisible toolsVisible)
{
   const bool showSuggestions = TabWidget::ToolVisibility::Suggestions & toolsVisible;
   Suggest::TreeView::setAllVisible(showSuggestions);

   structureWidget->setVisible(TabWidget::ToolVisibility::Structure & toolsVisible);
}

void Patch::Widget::slotSetPatchDigest()
{
   setDigest(&maxRef.header.digest, Max::RefStructure::PatchPart::Header);
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

void Patch::Widget::setDigest(Max::RefStructure::Digest* newDigest, const Max::RefStructure::PatchPart& part)
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

   topicInfo->setText(maxRef.partName(part));
   digestEdit->setText(digest->text);

   descriptionEdit->blockSignals(true);
   descriptionEdit->setText(digest->description);
   descriptionEdit->blockSignals(false);
}

void Patch::Widget::rebuild()
{
   Model::Abstract::rebuildAll();
   setDigest(&maxRef.header.digest, Max::RefStructure::PatchPart::Header);
}

void Patch::Widget::update()
{
   Model::Abstract::updateAll();
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

void Patch::Widget::setIcon(QLabel* iconLabel, Max::RefStructure::PatchPart part)
{
   iconLabel->setPixmap(maxRef.partIcon(part).pixmap(16, 16));
}
