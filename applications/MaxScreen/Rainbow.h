#ifndef RainbowH
#define RainbowH

#include <QColor>
#include <QMap>

class Rainbow
{
public:
   Rainbow(quint64 size);
   ~Rainbow();

public:
   static QColor getColor();
   void advanceColor();
   QColor lookup(quint64 value, bool wrap);

private:
   using ColorMap = QMap<quint64, QColor>;

private:
   struct Section
   {
      QColor color;
      int variableIndex;
      bool up;

      using List = QList<Section>;
   };

private:
   static Rainbow* me;
   quint64 size;
   ColorMap colorMap;

   quint64 currentIndex;
};

#endif // NOT RainbowH
