#include "SystemClipboard.h"

#include <AppKit/AppKit.h>

std::string SystemClipboard::read()
{
   NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
   if(!pasteboard)
      return "";

   const char* pasteboardContent = [[pasteboard stringForType:NSPasteboardTypeString] UTF8String];
   if(!pasteboardContent)
      return "";

   return std::string(pasteboardContent);
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
