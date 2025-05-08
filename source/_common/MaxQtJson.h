#ifndef MaxQtJsonH
#define MaxQtJsonH

namespace Max
{
   class QtJson
   {
   public:
      QtJson();

   protected:
      bool copyToDict(const QJsonObject& source, dict& target, std::string* error = nullptr);
   };
} // namespace Max

#ifndef MaxQtJsonHPP
#include "MaxQtJson.hpp"
#endif // NOT MaxQtJsonHPP

#endif // NOT MaxQtJsonH
