#include "Rainbow.h"

#include <QDebug>

Rainbow* Rainbow::me = nullptr;

Rainbow::Rainbow(quint64 size)
   : size(size)
   , colorMap()
   , currentIndex(0)
{
   me = this;

   static const Section::List sectionList = {
      {QColor(255, 0, 0), 1, true},
      {QColor(255, 255, 0), 0, false},
      {QColor(0, 255, 0), 2, true},
      {QColor(0, 255, 255), 1, false},
      {QColor(0, 0, 255), 0, true},
      {QColor(255, 0, 255), 2, false} //
   };

   const quint64 sectionSize = (size / sectionList.count());
   for (quint64 index = 0; index < size; index++)
   {
      const quint64 sectionRest = index % sectionSize;
      const quint64 sectionIndex = (index - sectionRest) / sectionSize;
      if (sectionIndex >= sectionList.count())
      {
         colorMap[index] = sectionList.first().color;
      }
      else
      {
         const Section& section = sectionList.at(sectionIndex);

         const quint8 value = (sectionRest * 255) / sectionSize;
         const quint8 colorValue = section.up ? value : 255 - value;

         QColor color = section.color;
         if (0 == section.variableIndex) // red
            color.setRed(colorValue);
         else if (1 == section.variableIndex) // green
            color.setGreen(colorValue);
         else // default blue
            color.setBlue(colorValue);

         colorMap[index] = color;
      }
   }
}

Rainbow::~Rainbow()
{
   me = nullptr;
}

QColor Rainbow::getColor()
{
   if (!me)
      return QColor(0, 0, 0);

   return me->colorMap.value(me->currentIndex);
}

void Rainbow::advanceColor()
{
   currentIndex++;
   if (currentIndex >= size)
      currentIndex = 0;
}

QColor Rainbow::lookup(quint64 value, bool wrap)
{
   quint64 index = wrap ? value % size : value;
   if (index >= size)
      index = size - 1;

   return colorMap.value(index);
}
