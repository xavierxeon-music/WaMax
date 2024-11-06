#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QToolBar>

#include "HelpForMax.h"
#include "MessageBar.h"
#include "Patch/PatchWidget.h"

MainWindow::MainWindow()
   : QMainWindow(nullptr)
   , packageWidget(nullptr)
   , patchWidget(nullptr)
   , schemaWidget(nullptr)
#ifdef TEST_CLIENT_AVAILABLE
   , testClient(nullptr)
#endif // TEST_CLIENT_AVAILABLE
{
   setWindowTitle("Help For Max [*]");
   setWindowIcon(QIcon(":/HelpForMax.svg"));

   patchWidget = new Patch::TabWidget(this);
   setCentralWidget(patchWidget);

   setStatusBar(new Message::Bar(this));

   auto addDock = [&](QWidget* widget, const Qt::DockWidgetArea& area, const QString& name)
   {
      QDockWidget* dockWidget = new QDockWidget(this);
      dockWidget->setObjectName(name);
      dockWidget->setWidget(widget);
      dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      dockWidget->setTitleBarWidget(new QWidget());

      addDockWidget(area, dockWidget);
   };

   packageWidget = new Package::TabWidget(this);
   addDock(packageWidget, Qt::LeftDockWidgetArea, "Package");

   schemaWidget = new Schema::Widget(this);
   addDock(schemaWidget, Qt::RightDockWidgetArea, "Schema");

   connect(patchWidget, &Patch::TabWidget::signalCheckDirty, this, &MainWindow::slotCheckDirty);
   connect(patchWidget, &Patch::TabWidget::signalCheckDirty, packageWidget, &Package::TabWidget::slotCheckDirty);

   connect(patchWidget, &Patch::TabWidget::signalTabSelected, schemaWidget, &Schema::Widget::slotLoad);
   connect(patchWidget, &Patch::TabWidget::signalRefWritten, packageWidget, &Package::TabWidget::slotRefWritten);
   connect(packageWidget, &Package::TabWidget::signalCloseAllPatches, patchWidget, &Patch::TabWidget::slotCloseAllPatches);
   connect(packageWidget, &Package::TabWidget::signalPatchSeleted, patchWidget, &Patch::TabWidget::slotShowPatch);

#ifdef TEST_CLIENT_AVAILABLE
   testClient = new TestClient;
   addDock(testClient, Qt::TopDockWidgetArea, "Test");
#endif // TEST_CLIENT_AVAILABLE

   populateMenuAndToolBar();

   packageWidget->init();
   patchWidget->init();

   QSettings settings;
   qDebug() << "SETTINGS @" << settings.fileName();
   restoreGeometry(settings.value("MainWidget/Geometry").toByteArray());
   restoreState(settings.value("MainWidget/State").toByteArray());
}

void MainWindow::slotCheckDirty()
{
   bool dirty = false;
   for (Patch::Widget* widget : findChildren<Patch::Widget*>())
   {
      dirty |= widget->isDirty();
   }

   setWindowModified(dirty);
}

void MainWindow::populateMenuAndToolBar()
{
   // patch
   QMenu* patchMenu = menuBar()->addMenu("Patch");

   // package
   QMenu* packageMenu = menuBar()->addMenu("Package");

   // view
   QMenu* viewMenu = menuBar()->addMenu("View");
   auto addViewToggle = [&](QWidget* widget, const QString& name, const QIcon& icon = QIcon())
   {
      auto toggleFunction = std::bind(&MainWindow::toogleDock, this, widget, name, std::placeholders::_1);
      QAction* viewAction = viewMenu->addAction(name, toggleFunction);
      viewAction->setCheckable(true);

      if (!icon.isNull())
         viewAction->setIcon(icon);

      QSettings dockSettings;
      const bool enabled = dockSettings.value("DockEnabled/" + name).toBool();

      widget->setVisible(enabled);
      viewAction->setChecked(enabled);

      const QSize size = dockSettings.value("DockSize/" + name).toSize();
      if (enabled && size.width() > 0 && size.height() > 0)
         widget->resize(size);

      return viewAction;
   };

   QAction* packageAction = addViewToggle(packageWidget, "Package", QIcon(":/PackageGeneral.svg"));
   QAction* schemmaAction = addViewToggle(schemaWidget, "Schema", QIcon(":/OverviewGeneral.svg"));
   schemmaAction->setShortcut(QKeySequence::Print);

#ifdef TEST_CLIENT_AVAILABLE
   addViewToggle(testClient, "TestClient");
#endif // TEST_CLIENT_AVAILABLE

   //
   auto spacer = [&]()
   {
      QWidget* widget = new QWidget(this);
      widget->setMinimumWidth(100);
      widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

      return widget;
   };

   QToolBar* toolBar = addToolBar("Toolbar");
   toolBar->setObjectName("Toolbar");
   toolBar->setMovable(false);

   packageWidget->populate(packageMenu, toolBar);
   toolBar->addSeparator();

   patchWidget->populate(patchMenu, viewMenu, toolBar);
   toolBar->addWidget(spacer());

   toolBar->addAction(packageAction);
   toolBar->addAction(schemmaAction);
}

void MainWindow::closeEvent(QCloseEvent* ce)
{
   QSettings settings;
   settings.setValue("MainWidget/Geometry", saveGeometry());
   settings.setValue("MainWidget/State", saveState());

   ce->accept();
}

void MainWindow::toogleDock(QWidget* widget, const QString& name, bool enabled)
{
   QSettings dockSettings;
   dockSettings.setValue("DockEnabled/" + name, enabled);
   if (!enabled)
   {
      dockSettings.setValue("DockSize/" + name, widget->size());
      widget->setVisible(false);
   }
   else
   {
      widget->setVisible(true);
      const QSize size = dockSettings.value("DockSize/" + name).toSize();
      if (size.width() > 0 && size.height() > 0)
         widget->resize(size);
   }
}

// main function

int main(int argc, char** argv)
{
   QApplication::setApplicationName("HelpForMax");
   QApplication::setOrganizationDomain("schweinesystem.ddns.org");
   QApplication::setOrganizationName("SchweineSystem");

   QSettings::setDefaultFormat(QSettings::IniFormat);

   QApplication app(argc, argv);

   // only allow one instance
   if (HelpForMax::isServerActive())
   {
      QMessageBox::critical(nullptr, "HelpForMax", "Only one running instance allowed");
      return 1;
   }

   MainWindow mw;
   mw.show();

   return app.exec();
}
