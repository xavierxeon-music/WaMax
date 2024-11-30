#ifndef SystemClipboardH
#define SystemClipboardH

#include <string>

struct SystemClipboard
{
   static std::string read();
   static void write(const std::string& text);
};

#endif // NOT ClipboardH
