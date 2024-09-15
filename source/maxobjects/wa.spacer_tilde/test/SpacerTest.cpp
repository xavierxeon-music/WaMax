#include "SpacerTest.h"

#include <QApplication>
#include <QCloseEvent>
#include <QSettings>

SpacerTest::SpacerTest()
   : QWidget(nullptr)
   , buffer()
   , cartesian()
{
   setupUi(this);
   setWindowTitle("Spacer Test");

   {
      QSettings settings;

      restoreGeometry(settings.value("SpacerTest/geometry").toByteArray());

      const double x = settings.value("Value/x").toDouble();
      const double y = settings.value("Value/y").toDouble();
      const double z = settings.value("Value/z").toDouble();

      xSpin->setValue(x);
      ySpin->setValue(y);
      zSpin->setValue(z);

      cartesian = Math::Vector3(x, y, z);
   }

   connect(xSpin, &QDoubleSpinBox::valueChanged, this, &SpacerTest::slotXChanged);
   connect(ySpin, &QDoubleSpinBox::valueChanged, this, &SpacerTest::slotYChanged);
   connect(zSpin, &QDoubleSpinBox::valueChanged, this, &SpacerTest::slotZChanged);

   update();
}

void SpacerTest::slotXChanged(const double& value)
{
   cartesian.setA(value);
   update();
   {
      QSettings settings;
      settings.setValue("Value/x", value);
   }
}

void SpacerTest::slotYChanged(const double& value)
{
   cartesian.setB(value);
   update();
   {
      QSettings settings;
      settings.setValue("Value/y", value);
   }
}

void SpacerTest::slotZChanged(const double& value)
{
   cartesian.setC(value);
   update();
   {
      QSettings settings;
      settings.setValue("Value/z", value);
   }
}

void SpacerTest::update()
{
   const Math::Vector3 spherical = cartesian.cart2Sphre();

   const double az = spherical.getA();
   const double el = spherical.getB();

   azSpin->setValue(az);
   elSpin->setValue(el);
   radSpin->setValue(spherical.getC());

   impulseChartView->update(az, el);
}

void SpacerTest::closeEvent(QCloseEvent* ce)
{
   QSettings settings;

   settings.setValue("SpacerTest/geometry", saveGeometry());

   ce->accept();
}

// main function

int main(int argc, char** argv)
{
   QSettings::setDefaultFormat(QSettings::IniFormat);

   QApplication::setApplicationName("Spacer Test");
   QApplication::setOrganizationDomain("eu.schweinesystem");

   QApplication app(argc, argv);

   SpacerTest mw;
   mw.show();

   return app.exec();
}
