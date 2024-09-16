#include "SignalChart.h"

#include <QValueAxis>

SignalChart::SignalChart(QWidget* parent)
   : QChartView(parent)
{
   QValueAxis* axisX = new QValueAxis;
   axisX->setRange(0, 2.0);
   //axisX->setLabelFormat("%g");
   axisX->setTitleText("Seconds");
   chart()->addAxis(axisX, Qt::AlignBottom);

   QValueAxis* axisY = new QValueAxis;
   axisY->setRange(-1, 1);
   axisY->setTitleText("level");
   chart()->addAxis(axisY, Qt::AlignLeft);
}
