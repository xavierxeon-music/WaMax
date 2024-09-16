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
   void update(const double& az, const double& el, const double& frequency);

private:
   Spatial::RingBuffer buffer;
   QLineSeries* inputSignal;
   QLineSeries* leftEar;
   QLineSeries* rightEar;
};

#endif // NOT SignalChartH
