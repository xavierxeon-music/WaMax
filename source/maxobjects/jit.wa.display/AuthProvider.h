#ifndef FlexDisplayAuthProviderH
#define FlexDisplayAuthProviderH

#include <AuthProviderToken.h>

namespace FlexDisplay
{
   class AuthProvider : public ::AuthProvider::Token
   {
      Q_OBJECT

   public:
      AuthProvider(QObject* parent);

   private:
      void setAuthorization(QNetworkRequest& request) const override;
   };
} // namespace FlexDisplay

#endif // NOT FlexDisplayAuthProviderH
