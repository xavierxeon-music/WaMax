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

#include "Package/PackageInfo.h"
#include "PatchModelArgument.h"
#include "PatchModelHeader.h"
#include "PatchModelNamedMessage.h"
#include "PatchModelOutput.h"
#include "PatchModelTypedMessage.h"
#include "PatchTabWidget.h"

Patch::Widget::Widget(TabWidget* tabWidget, const Package::Info* info, const QString& patchFileName)
   : QWidget(tabWidget)
   , Structure()
   , tabWidget(tabWidget)
   , info(info)
   , path(patchFileName)
   , name()
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

      setIcon(patchIcon, Structure::PatchPart::Header);
      setIcon(argumentIcon, Structure::PatchPart::Argument);
      setIcon(typedMessageIcon, Structure::PatchPart::MessageTyped);
      setIcon(attributeIcon, Structure::PatchPart::Attribute);
      setIcon(nameMessageIcon, Structure::PatchPart::MessageNamed);
      setIcon(outputIcon, Structure::PatchPart::Output);

      // set models
      Model::Header* headerModel = new Model::Header(this, this);
      modelList.append(headerModel);
      headerTree->init(this, headerModel, 1);
      headerTree->setItemDelegateForColumn(0, new Delegate::PatchType(this, headerModel));

      Model::Argument* argumentModel = new Model::Argument(this, this);
      modelList.append(argumentModel);
      argumentTree->init(this, argumentModel);
      argumentTree->setButtons(argumentAddButton, argumentRemoveButton);
      argumentTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentModel));

      Model::TypedMessage* typedMessageModel = new Model::TypedMessage(this, this);
      modelList.append(typedMessageModel);
      typedMessageTree->init(this, typedMessageModel);

      Model::NamedMessage* namedMessageModel = new Model::NamedMessage(this, this);
      modelList.append(namedMessageModel);
      namedMessageTree->init(this, namedMessageModel);
      namedMessageTree->setButtons(namedMessageAddButton, namedMessageRemoveButton);
      namedMessageTree->setItemDelegateForColumn(1, new Delegate::DataType(this, namedMessageModel));

      Model::Output* outputModel = new Model::Output(this, this);
      modelList.append(outputModel);
      outputTree->init(this, outputModel);
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
   QHBoxLayout* masterLayout = new QHBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(scrollArea);
   masterLayout->addWidget(editArea);

   // load content
   name = info->extractPatchName(path);
   propagateDirty(false);

   File::Ref(this, info).read(name);
   rebuild();
}

Patch::Widget::~Widget()
{
   tabWidget->forceDirtyCheck();
}

const QString& Patch::Widget::getPath() const
{
   return path;
}

const Package::Info* Patch::Widget::getPacakgeInfo() const
{
   return info;
}

void Patch::Widget::writeRef()
{
   File::Ref(this, info).write(name);
   File::Help(this, info).write(name);
   File::Init(this, info).write(name);
   propagateDirty(false);
}

void Patch::Widget::openInMax()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Patch::Widget::openXML()
{
   const QString refPath = File::Ref(this, info).getFilePath(name);
   QDesktopServices::openUrl(QUrl::fromLocalFile(refPath));
}

bool Patch::Widget::isDirty() const
{
   return dirty;
}

void Patch::Widget::slotSetPatchDigest()
{
   setDigest(&header.digest, Structure::PatchPart::Header);
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

   setDigest(&header.digest, Structure::PatchPart::Header);

   patchNameLabel->setText(name);
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
   static const QString bullet = QString::fromUtf8("\u25cf") + QString(" ");
   if (isDirty)
   {
      dirty = true;
      setWindowTitle(bullet + name);
   }
   else
   {
      dirty = false;
      setWindowTitle(name);
   }

   tabWidget->forceDirtyCheck();
}

void Patch::Widget::setIcon(QLabel* iconLabel, Structure::PatchPart part)
{
   iconLabel->setPixmap(partIcon(part).pixmap(16, 16));
}
