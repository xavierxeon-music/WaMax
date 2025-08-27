#include "FlexDisplayAuthProvider.h"

FlexDisplay::AuthProvider::AuthProvider(QObject* parent)
   : ::AuthProvider::Token(parent)
{
}

void FlexDisplay::AuthProvider::setAuthorization(QNetworkRequest& request) const
{
   static const QByteArray bearerToken = "asdasdjasd";
   request.setRawHeader("Authorization", "Bearer " + bearerToken);
}
