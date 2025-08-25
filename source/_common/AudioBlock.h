#ifndef AudioBlockH
#define AudioBlockH

#include <QString>

#include <QSharedMemory>

class AudioBlock
{
public:
   class List : public QList<AudioBlock*>
   {
   public:
      List() = default;
      ~List();

   private:
      bool clearOnExit;
   };

public:
   AudioBlock(const QString& name, int counter, bool readOnly);
   ~AudioBlock();

public:
   void copyFrom(const double* data, const size_t& size);
   void copyTo(double* data, const size_t& size);
   const QString& getErrorString() const;

private:
   constexpr static size_t BlockSize = 2048;
   struct Data
   {
      double data[BlockSize]; // needs to be stack memmory, choose suffiently large size
   };

private:
   QSharedMemory sharedMemory;
   const bool readOnly;
   Data* sharedData;
   size_t cursor;
   QString errorString;
};

#ifndef AudioBlockHPP
#include "AudioBlock.hpp"
#endif // NOT AudioBlockHPP

#endif // NOT AudioBlockH
