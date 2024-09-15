#ifndef ImpulseChartH
#define ImpulseChartH

#include <QChartView>

#include <QLineSeries>

class ImpulseChart : public QChartView
{
   Q_OBJECT

public:
   ImpulseChart(QWidget* parent);

public:
   void update(const double& az, const double& el);

private:
   QLineSeries* leftEar;
   QLineSeries* rightEar;
};

#endif // NOT ImpulseChartH
