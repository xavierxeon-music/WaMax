#ifndef SizeH
#define SizeH

#include <QDataStream>
#include <QSize>

class Size
{
public:
   Size();

public:
   bool isValid() const;
   void update(const int width, const int height);
   void dump(QDataStream& stream);
   void load(QDataStream& stream);
   const int& getWidth() const;
   const int& getHeight() const;
   operator QSize() const;

private:
   int width;
   int height;
};

#ifndef SizeHPP
#include "Size.hpp"
#endif // NOT SizeHPP

#endif // NOT SizeH
