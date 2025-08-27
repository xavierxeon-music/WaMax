
#include "jit.wa.display.h"

#include <QBuffer>

#include <FileTools.h>

#include <MaxPatcher.h>

JitDisplay::JitDisplay(const atoms& args)
   : object<JitDisplay>()
   , matrix_operator<>(false)
   , inputMatrix{this, "(matrix) Input", "matrix"}
   , outputMatrix{this, "(matrix) passthrough", "matrix"}
   , name{this, "name", "dummy"}
   , jitClassSetup{this, "jitclass_setup", Max::Patcher::minBind(this, &JitDisplay::jitClassSetupFunction)}
   , image()
   , bearerToken()
   , setInfoMap()
   , mode(Mode::Info)
{
   const QJsonObject apiContent = FileTools::readApiKeys("flexdisplay");
   bearerToken = apiContent["token"].toString().toUtf8();

   curl_global_init(CURL_GLOBAL_DEFAULT);

   requestInfo();
}

JitDisplay::~JitDisplay()
{
   curl_global_cleanup();
}

template <typename matrix_type>
matrix_type JitDisplay::calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position)
{
   return matrix_type{};
}

pixel JitDisplay::calc_cell(pixel input, const matrix_info& info, matrix_coord& position)
{
   const int w = info.width();
   const int h = info.height();
   if (image.width() != w || image.height() != h)
   {
      cout << "resize image to " << w << "x" << h << endl;
      image = QImage(w, h, QImage::Format_ARGB32);
   }

   const int x = position.x();
   const int y = position.y();
   const QRgb color = qRgb(input[red], input[green], input[blue]);

   QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(y));
   line[x] = color;

   return input;
}

atoms JitDisplay::jitClassSetupFunction(const atoms& args, const int inlet)
{
   using namespace c74;

   // add mop
   auto mop = (max::t_jit_object*)max::jit_object_new(max::_jit_sym_jit_mop, 1, 1);
   max::jit_class_addadornment(this_jit_class, mop);

   // add methods
   max::jit_class_addmethod(this_jit_class, reinterpret_cast<max::method>(jit_matrix_calc<JitDisplay>), "matrix_calc", max::A_CANT, 0);
   max::jit_class_addmethod(this_jit_class, reinterpret_cast<max::method>(JitDisplay::maxBangMessage), "bang", A_DEFER_LOW, 0);

   return {};
}

long JitDisplay::maxBangMessage(c74::max::t_object* x, long index, long count)
{
   minwrap<JitDisplay>* ob = (minwrap<JitDisplay>*)(x);
   ob->m_min_object.sendTexture();
   return 0;
}

void JitDisplay::requestInfo()
{
   const QJsonObject jsonObject = post("/api/info", QByteArray());
   compileInfo(jsonObject);
}

void JitDisplay::sendTexture()
{
   if (Mode::Upload != mode)
      return;

   if (!setInfoMap.contains("music"))
      return;

   const QString target(name.get());
   const SetInfo setInfo = setInfoMap.value("music");
   const QString imageId = setInfo.images.value(target);

   QJsonObject payload;
   payload["set_id"] = setInfo.id;
   payload["image_id"] = imageId;
   payload["name"] = target;

   QByteArray data;
   QBuffer buffer(&data);
   buffer.open(QIODevice::WriteOnly);
   image.save(&buffer, "PNG"); // writes image into ba in PNG format

   const QByteArray image64Data = data.toBase64();
   payload["image64"] = QString::fromUtf8(image64Data);

   cout << "send texture " << target.toStdString() << endl;

   const QByteArray uploadContent = QJsonDocument(payload).toJson(QJsonDocument::Compact);
   const QJsonObject jsonObject = post("/api/image", uploadContent);
   updateInfo(jsonObject);
}

void JitDisplay::compileInfo(const QJsonObject& info)
{
   for (QJsonObject::const_iterator iterSet = info.constBegin(); iterSet != info.constEnd(); iterSet++)
   {
      const QString& setId = iterSet.key();
      const QJsonObject setObject = iterSet.value().toObject();

      SetInfo setInfo;
      setInfo.id = setId;

      const QJsonObject imagesObject = setObject["images"].toObject();
      for (QJsonObject::const_iterator iterImage = imagesObject.constBegin(); iterImage != imagesObject.constEnd(); iterImage++)
      {
         const QString& imageId = iterImage.key();
         const QString imageName = iterImage.value().toString();
         setInfo.images.insert(imageName, imageId);
      }
      const QString setName = setObject["name"].toString();
      setInfoMap.insert(setName, setInfo);
   }

   for (SetInfo::Map::const_iterator iterSet = setInfoMap.constBegin(); iterSet != setInfoMap.constEnd(); iterSet++)
   {
      const QString& setName = iterSet.key();
      const SetInfo& setInfo = iterSet.value();
      const QStringList imageNames = setInfo.images.keys();

      cout << "Set: " << setName.toStdString() << ", Images: " << imageNames.join(", ").toStdString() << endl;
   }

   mode = Mode::Upload;
}

void JitDisplay::updateInfo(const QJsonObject& info)
{
   if (info["success"].toBool())
   {
      const QString setId = info["set_id"].toString();
      const QString imageId = info["image_id"].toString();

      SetInfo& setInfo = setInfoMap["music"];
      if (setInfo.id != setId)
      {
         cerr << "Set ID mismatch!" << endl;
         return;
      }

      const QString target(name.get());
      setInfo.images.insert(target, imageId);
   }
}

QJsonObject JitDisplay::post(const QString& endpoint, const QByteArray& content)
{
   std::string response;
   const QByteArray url = "https://schweinesystem.ddns.net/flexdisplay" + endpoint.toUtf8();
   //const QByteArray url = "http://127.0.0.1:5000" + endpoint.toUtf8();

   struct curl_slist* headers = NULL;

   curl = curl_easy_init();
   curl_easy_setopt(curl, CURLOPT_URL, url.constData());
   curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, bearerToken.constData());
   curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "application/json");
   curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);

   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.constData());
   curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, content.size());

   headers = curl_slist_append(headers, "Content-Type: application/json");
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, JitDisplay::curlWriteCallback);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

   auto cleanup = [&]()
   {
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
   };

   CURLcode res = curl_easy_perform(curl);
   if (CURLE_OK != res)
   {
      cerr << __FUNCTION__ << " failed: " << curl_easy_strerror(res) << endl;
      cleanup();
      return QJsonObject();
   }
   cleanup();

   const QByteArray data = QByteArray::fromStdString(response);
   const QJsonObject jsonObject = FileTools::parseBytes(data);

   return jsonObject;
}

size_t JitDisplay::curlWriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
   size_t totalSize = size * nmemb;
   ((std::string*)userp)->append((char*)contents, totalSize);
   return totalSize;
}

MIN_EXTERNAL(JitDisplay);