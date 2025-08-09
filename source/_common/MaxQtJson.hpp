#ifndef MaxQtJsonHPP
#define MaxQtJsonHPP

#include "MaxQtJson.h"

inline Max::QtJson::QtJson()
{
   using namespace c74::max;
   common_symbols_init();
}

void Max::QtJson::copyToMaxDict(const QJsonObject& source, dict& target, std::string* error) const
{
   const QJsonDocument doc(source);
   const QByteArray jsonData = doc.toJson();

   using namespace c74::max;
   t_atom result[1];

   t_object* jsonreader = (t_object*)object_new(_sym_nobox, _sym_jsonreader); // can not reuse this object
   t_object* dictObject = nullptr;

   auto cleanup = [&]()
   {
      if (jsonreader)
         object_free(jsonreader);
      if (dictObject)
         object_free(dictObject);
   };

   t_max_err parserError = (t_max_err)object_method(jsonreader, _sym_parse, jsonData.constData(), result);
   if (parserError)
   {
      if (error)
         *error = "error parsing json data";
      return cleanup();
   }

   dictObject = (t_object*)atom_getobj(result);
   if (!dictObject)
   {
      if (error)
         *error = "error getting dictionary from json data";
      return cleanup();
   }

   if (object_classname_compare(dictObject, _sym_dictionary))
   {
      target.clear();
      target = dict{(t_dictionary*)dictObject};
      target.touch();
   }
   else
   {
      if (error)
         *error = "error: object is not a dictionary";
      return cleanup();
   }

   cleanup();
}

void Max::QtJson::mergeDicts(const QJsonObject& source, QJsonObject& target) const
{
   for (const QString& key : source.keys())
   {
      target[key] = source[key];
   }
}

QJsonObject Max::QtJson::fromMaxDict(const dict& source) const
{
   using namespace c74::max;

   t_object* jsonwriter = (t_object*)object_new(_sym_nobox, _sym_jsonwriter); // can not reuse this object
   if (!jsonwriter)
      return QJsonObject();

   t_object* object = source;
   t_dictionary* maxdict = (t_dictionary*)object;
   object_method(jsonwriter, _sym_writedictionary, maxdict);

   t_handle json;
   object_method(jsonwriter, _sym_getoutput, &json);

   const QByteArray data((const char*)*json);
   object_free(jsonwriter);

   QJsonObject result = QJsonDocument::fromJson(data).object();
   return result;
}

#endif // NOT MaxQtJsonHPP
