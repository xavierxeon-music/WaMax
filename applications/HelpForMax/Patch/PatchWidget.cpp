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
#include "PatchRefModelArgument.h"
#include "PatchRefModelHeader.h"
#include "PatchRefModelNamedMessage.h"
#include "PatchRefModelOutput.h"
#include "PatchRefModelTypedMessage.h"
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

      setIcon(patchIcon, Ref::Structure::PatchPart::Header);
      setIcon(argumentIcon, Ref::Structure::PatchPart::Argument);
      setIcon(typedMessageIcon, Ref::Structure::PatchPart::MessageTyped);
      setIcon(attributeIcon, Ref::Structure::PatchPart::Attribute);
      setIcon(nameMessageIcon, Ref::Structure::PatchPart::MessageNamed);
      setIcon(outputIcon, Ref::Structure::PatchPart::Output);
      setIcon(otherIcon, Ref::Structure::PatchPart::Other);

      // set suggest models
      Suggest::Model::Argument* argumentSuggestModel = new Suggest::Model::Argument(this, maxRef, maxPatch);
      argumentSuggestTree->init(this, argumentSuggestModel);
      argumentSuggestTree->setButton(argumnetTransferButton);

      Suggest::Model::TypedMessage* typedMessageSuggestModel = new Suggest::Model::TypedMessage(this, maxRef, maxPatch);
      typedMessageSuggestTree->init(this, typedMessageSuggestModel);
      typedMessageSuggestTree->setButton(typedMessageTransferButton);

      Suggest::Model::NamedMessage* namedMessageSuggestModel = new Suggest::Model::NamedMessage(this, maxRef, maxPatch);
      namedMessageSuggestTree->init(this, namedMessageSuggestModel);
      namedMessageSuggestTree->setButton(namedMessageTransferButton);

      // set reference models
      PatchRef::Model::Header* headerPatchModel = new PatchRef::Model::Header(this, maxRef);
      headerPatchTree->init(this, headerPatchModel, 1);
      headerPatchTree->setItemDelegateForColumn(0, new Delegate::PatchType(this, headerPatchModel));

      PatchRef::Model::Argument* argumentPatchModel = new PatchRef::Model::Argument(this, maxRef);
      argumentPatchTree->init(this, argumentPatchModel);
      argumentPatchTree->setButtons(argumentAddButton, argumentRemoveButton);
      argumentPatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentPatchModel));

      PatchRef::Model::TypedMessage* typedMessagPatcheModel = new PatchRef::Model::TypedMessage(this, maxRef);
      typedMessagePatchTree->init(this, typedMessagPatcheModel);

      PatchRef::Model::NamedMessage* namedMessagePatchModel = new PatchRef::Model::NamedMessage(this, maxRef);
      namedMessagePatchTree->init(this, namedMessagePatchModel);
      namedMessagePatchTree->setButtons(namedMessageAddButton, namedMessageRemoveButton);
      namedMessagePatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, namedMessagePatchModel));

      PatchRef::Model::Output* outputPatchModel = new PatchRef::Model::Output(this, maxRef);
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

   structureWidget = new Max::Widget(this);

   addWidget(scrollArea);
   addWidget(editArea);
   addWidget(structureWidget);

   // load content
   patchInfo = packageInfo->extractPatchInfo(path);
   propagateDirty(false);

   maxPatch.readPatch(path);
   structureWidget->load(maxPatch);

   maxRef.dirtyHook = std::bind(&Widget::setDirty, this);

   File::RefXML(packageInfo, maxRef).read(patchInfo);
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
   File::RefXML(packageInfo, maxRef).write(patchInfo);
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
   const QString refPath = File::RefXML(packageInfo, maxRef).getFilePath(patchInfo);
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
   setDigest(&maxRef.header.digest, Ref::Structure::PatchPart::Header);
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

void Patch::Widget::setDigest(Ref::Structure::Digest* newDigest, const Ref::Structure::PatchPart& part)
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
   PatchRef::Model::Abstract::rebuildAll();
   Suggest::Model::Abstract::rebuildAll();
   setDigest(&maxRef.header.digest, Ref::Structure::PatchPart::Header);
}

void Patch::Widget::update()
{
   PatchRef::Model::Abstract::updateAll();
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

void Patch::Widget::setIcon(QLabel* iconLabel, Ref::Structure::PatchPart part)
{
   iconLabel->setPixmap(maxRef.partIcon(part).pixmap(16, 16));
}
