#ifndef SharedDataH
#define SharedDataH

#include <QSharedMemory>

template <typename DataBlock>
class SharedData
{
public:
   class List : public QList<DataBlock*>
   {
   public:
      List() = default;
      ~List();
   };

public:
   SharedData(const QString& uniqueName, bool readOnly);
   virtual ~SharedData();

public:
   const QString& getErrorString() const;

protected:
   virtual void clear(DataBlock* block);

protected:
   DataBlock* dataBlock;

private:
   QSharedMemory sharedMemory;
   const bool readOnly;
   QString errorString;
};

#ifndef SharedDataHPP
#include "SharedData.hpp"
#endif // NOT SharedDataHPP

#endif // NOT SharedDataH
