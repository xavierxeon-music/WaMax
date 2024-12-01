#ifndef ScreenSizeH
#define ScreenSizeH

#include <QDataStream>

class ScreenSize
{
public:
   ScreenSize();

public:
   void update(const int width, const int height);
   void dump(QDataStream& stream);
   void load(QDataStream& stream);
   const int& getWidth() const;
   const int& getHeight() const;

private:
   int width;
   int height;
};

#ifndef ScreenSizeHPP
#include "ScreenSize.hpp"
#endif // NOT ScreenSizeHPP

#endif // NOT ScreenSizeH
