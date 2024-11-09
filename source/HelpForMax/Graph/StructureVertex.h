#ifndef StructureVertexH
#define StructureVertexH

#include <QList>

namespace Structure
{
   class Vertex
   {
   public:
      using PrtList = QList<Vertex*>;
      using ConstPtrList = QList<const Vertex*>;

   public:
      Vertex(const QString& name = QString());

   public:
      const QString& getName() const;

   protected:
      QString name;
   };
} // namespace Structure

#endif // NOT GraphAbstractVertexH
