#ifndef ImpulseChartH
#define ImpulseChartH

#include <QChartView>

#include <QLineSeries>

#include <SpatialCoords.h>

class ImpulseChart : public QChartView
{
   Q_OBJECT

public:
   ImpulseChart(QWidget* parent);

public:
   void update(const Spatial::Coords& coords);

private:
   QLineSeries* leftEar;
   QLineSeries* rightEar;
};

#endif // NOT ImpulseChartH
