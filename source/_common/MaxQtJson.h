#ifndef MaxQtJsonH
#define MaxQtJsonH

namespace Max
{
   class QtJson
   {
   public:
      QtJson();

   protected:
      void copyToMaxDict(const QJsonObject& source, dict& target, std::string* error = nullptr) const;
      void mergeDicts(const QJsonObject& source, QJsonObject& target) const;
   };
} // namespace Max

#ifndef MaxQtJsonHPP
#include "MaxQtJson.hpp"
#endif // NOT MaxQtJsonHPP

#endif // NOT MaxQtJsonH
