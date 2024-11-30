#include "SystemClipboard.h"

#include <AppKit/AppKit.h>

std::string SystemClipboard::read()
{
   NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
   if(!pasteboard)
      return "";

   const std::string text = std::string([[pasteboard stringForType:NSPasteboardTypeString] UTF8String]);
   return text;
}

void SystemClipboard::write(const std::string& text)
{
   NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
   if(!pasteboard)
      return;
   NSString *string = [NSString stringWithUTF8String:text.c_str()];

   [pasteboard clearContents];
   [pasteboard setString:string forType:NSPasteboardTypeString];
}
