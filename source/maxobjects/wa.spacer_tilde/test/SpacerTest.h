#ifndef SpacerTestH
#define SpacerTestH

#include "ui_SpacerTest.h"
#include <QWidget>

#include <QLineSeries>

#include "MathVector3.h"
#include "SpatialRingBuffer.h"

class SpacerTest : public QWidget, private Ui::SpacerTest
{
   Q_OBJECT

public:
   SpacerTest();

private slots:
   void slotXChanged(const double& value);
   void slotYChanged(const double& value);
   void slotZChanged(const double& value);

private:
   void update();
   void closeEvent(QCloseEvent* ce) override;

private:
   Spatial::RingBuffer buffer;
   Math::Vector3 cartesian;
};

#endif // NOT SpacerTestH
