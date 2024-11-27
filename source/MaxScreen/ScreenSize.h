#ifndef ScreenSizeH
#define ScreenSizeH

#include <QDataStream>

class ScreenSize
{
public:
   ScreenSize();

public:
   void update(const int width, const int height);
   void save(QDataStream& stream);
   void load(QDataStream& stream);

private:
   int width;
   int height;
};

#ifndef ScreenSizeHPP
#include "ScreenSize.hpp"
#endif // NOT ScreenSizeHPP

#endif // NOT ScreenSizeH
