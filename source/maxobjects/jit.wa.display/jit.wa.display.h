#ifndef DisplayH
#define DisplayH

#include "c74_min.h"
using namespace c74::min;
#include <CommonQt.h>

#include <curl/curl.h>

#include <QImage>
#include <QJsonObject>
#include <QMap>

class JitDisplay : public object<JitDisplay>, public matrix_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   JitDisplay(const atoms& args = {});
   ~JitDisplay();

public:
   template <typename matrix_type>
   matrix_type calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position);

   pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position);

private:
   enum class Mode
   {
      Info,
      Upload
   };

   struct SetInfo
   {
      QString id;
      QMap<QString, QString> images; // name vs id

      using Map = QMap<QString, SetInfo>; // name vs set
   };

private:
   atoms jitClassSetupFunction(const atoms& args, const int inlet);
   static long maxBangMessage(c74::max::t_object* x, long index, long count);

   void requestInfo();
   void sendTexture();
   void uploadTexture(const QJsonObject payload);
   void compileInfo(const QJsonObject& info);
   void updateInfo(const QJsonObject& info);

   QJsonObject post(const QString& endpoint, const QByteArray& content);
   static size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

private:
   inlet<> inputMatrix;
   outlet<> outputMatrix;
   attribute<symbol> name;
   message<> jitClassSetup;

   QImage image;
   QByteArray bearerToken;
   SetInfo::Map setInfoMap;
   Mode mode;
};

#endif // DisplayH
