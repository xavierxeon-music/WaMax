#ifndef ApiKeysH
#define ApiKeysH

#include "c74_min.h"
using namespace c74::min;

#include <CommonQt.h>

#include <QJsonObject>

class ApiKeys : public object<ApiKeys>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   ApiKeys(const atoms& args = {});

private:
   atoms contentFunction(const atoms& args, const int inlet);
   atoms queryFunction(const atoms& args, const int inlet);
   void readApiKeys(const QString& what);

private:
   inlet<> input;
   outlet<> query;
   outlet<> content;

   message<> bangMessage;
   message<> queryMessage;

   QJsonObject apiContent;
   dict dictApi;
};

#endif // ApiKeysH
