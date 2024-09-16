#include "SignalChart.h"

#include <QSettings>
#include <QValueAxis>

#include "MathGeneral.h"

static const uint16_t samplesPerSecond = 1000;

SignalChart::SignalChart(QWidget* parent)
   : QChartView(parent)
   , buffer()
   , inputSignal(nullptr)
   , leftEar(nullptr)
   , rightEar(nullptr)
{
   QValueAxis* axisX = new QValueAxis;
   axisX->setRange(0, 2.0);
   axisX->setTitleText("Seconds");
   chart()->addAxis(axisX, Qt::AlignBottom);

   QValueAxis* axisY = new QValueAxis;
   axisY->setRange(-1, 1);
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

   inputSignal = addSeries("Input", QColor(Qt::blue));
   leftEar = addSeries("Left", QColor(Qt::green));
   rightEar = addSeries("Right", QColor(Qt::red));

   chart()->legend()->setAlignment(Qt::AlignRight);
}

void SignalChart::update(const double& az, const double& el, const double& frequency)
{
   inputSignal->clear();
   leftEar->clear();
   rightEar->clear();

   const double timePerSample = 1.0 / samplesPerSecond;
   double time = 0.0;

   while (time < 2.0)
   {
      const double angle = 2.0 * M_PI * frequency * time;
      const double signal = std::sin(angle);
      inputSignal->append(time, signal);

      buffer.add(signal, az, el);

      const double left = buffer.convolve(true);
      leftEar->append(time, left);

      const double right = buffer.convolve(false);
      rightEar->append(time, right);

      time += timePerSample;
   }
}
