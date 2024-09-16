#ifndef SignalChartH
#define SignalChartH

#include <QChartView>

#include <QLineSeries>

#include "SpatialCoords.h"
#include "SpatialRingBuffer.h"

class SignalChart : public QChartView
{
   Q_OBJECT

public:
   SignalChart(QWidget* parent);

public:
   void setParams(const Spatial::Coords& coords, const double& frequency);

private:
   void timerEvent(QTimerEvent* event) override;
   void initSeries();

private:
   Spatial::Coords coords;
   double frequency;
   uint32_t currentIndex;

   Spatial::RingBuffer buffer;
   QLineSeries* inputSignal;
   QLineSeries* leftEar;
   QLineSeries* rightEar;
};

#endif // NOT SignalChartH
