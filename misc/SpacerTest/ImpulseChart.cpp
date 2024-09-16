#include "ImpulseChart.h"

#include <QValueAxis>

#include "SpatialFunction.h"

ImpulseChart::ImpulseChart(QWidget* parent)
   : QChartView(parent)
   , leftEar(nullptr)
   , rightEar(nullptr)
{
   QValueAxis* axisX = new QValueAxis;
   axisX->setRange(0, 128);
   axisX->setLabelFormat("%g");
   axisX->setTitleText("Samples");
   chart()->addAxis(axisX, Qt::AlignBottom);

   QValueAxis* axisY = new QValueAxis;
   axisY->setRange(0, 1);
   axisY->setTitleText("level");
   chart()->addAxis(axisY, Qt::AlignLeft);

   auto addSeries = [&](const QString& name, const QColor& color)
   {
      QLineSeries* series = new QLineSeries;

      chart()->addSeries(series);
      series->attachAxis(axisX);
      series->attachAxis(axisY);

      series->setName(name);
      series->setColor(color);

      return series;
   };

   leftEar = addSeries("Left", QColor(Qt::green));
   rightEar = addSeries("Right", QColor(Qt::red));

   chart()->legend()->setAlignment(Qt::AlignRight);
}

void ImpulseChart::update(const double& az, const double& el)
{
   Spatial::Function leftFunction(az, el, true);
   Spatial::Function rightFunction(az, el, false);

   leftEar->clear();
   rightEar->clear();

   for (int index = 0; index < 128; index++)
   {
      const double leftValue = leftFunction.value(index);
      leftEar->append(index, leftValue);

      const double rightValue = rightFunction.value(index);
      rightEar->append(index, rightValue);
   }
}
