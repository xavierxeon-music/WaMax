#include "SignalChart.h"

#include <QSettings>
#include <QValueAxis>

#include "MathGeneral.h"

static const uint16_t samplesPerSecond = 1000;
static const double timerIntervall = 0.01;
static const uint32_t xLength = 5;

static const uint32_t maxPointCount = xLength * samplesPerSecond;
static const uint32_t pointsPerIntervall = static_cast<int>(timerIntervall * samplesPerSecond);

SignalChart::SignalChart(QWidget* parent)
   : QChartView(parent)
   , az(90.0)
   , el(0.0)
   , frequency(1.0)
   , currentIndex(0)
   , buffer()
   , inputSignal(nullptr)
   , leftEar(nullptr)
   , rightEar(nullptr)
{
   QValueAxis* axisX = new QValueAxis;
   axisX->setRange(0, xLength);
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

   initSeries();
   startTimer(timerIntervall * 1000);
}

void SignalChart::setParams(const double& az, const double& el, const double& frequency)
{
   this->az = az;
   this->el = el;
   this->frequency = frequency;
}

void SignalChart::timerEvent(QTimerEvent* event)
{
   Q_UNUSED(event)

   const uint32_t target = currentIndex + pointsPerIntervall;
   while (currentIndex < target)
   {
      const uint32_t index = currentIndex % maxPointCount;
      currentIndex++;

      const double time = static_cast<double>(index * xLength) / static_cast<double>(maxPointCount);

      const double angle = 2.0 * M_PI * frequency * time;
      const double signal = std::sin(angle);

      buffer.add(signal, az, el);
      const double left = buffer.convolve(true);
      const double right = buffer.convolve(false);

      inputSignal->replace(index, time, signal);
      leftEar->replace(index, time, left);
      rightEar->replace(index, time, right);
   }

   while (currentIndex > maxPointCount)
      currentIndex -= maxPointCount;
}

void SignalChart::initSeries()
{
   inputSignal->clear();
   leftEar->clear();
   rightEar->clear();

   for (uint32_t index = 0; index <= maxPointCount; index++)
   {
      const double time = static_cast<double>(index * xLength) / static_cast<double>(maxPointCount);

      const double angle = 2.0 * M_PI * frequency * time;
      const double signal = std::sin(angle);

      buffer.add(signal, az, el);
      const double left = buffer.convolve(true);
      const double right = buffer.convolve(false);

      inputSignal->append(time, signal);
      leftEar->append(time, left);
      rightEar->append(time, right);
   }
}
