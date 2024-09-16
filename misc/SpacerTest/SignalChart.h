#ifndef SignalChartH
#define SignalChartH

#include <QChartView>

#include <QLineSeries>

#include "SpatialRingBuffer.h"

class SignalChart : public QChartView
{
   Q_OBJECT

public:
   SignalChart(QWidget* parent);

public:
   void setParams(const double& az, const double& el, const double& frequency);

private:
   void timerEvent(QTimerEvent* event) override;
   void initSeries();

private:
   double az;
   double el;
   double frequency;
   uint32_t currentIndex;

   Spatial::RingBuffer buffer;
   QLineSeries* inputSignal;
   QLineSeries* leftEar;
   QLineSeries* rightEar;
};

#endif // NOT SignalChartH
