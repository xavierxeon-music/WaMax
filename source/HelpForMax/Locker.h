#ifndef LockerH
#define LockerH

#include <algorithm>

template <size_t N>
struct CompileTimeString
{
   constexpr CompileTimeString(const char (&str)[N]);
   char value[N];
};

template <CompileTimeString tag>
class Locker
{
public:
   Locker();
   ~Locker();

public:
   static bool engaged();

private:
   static bool lock;
};

#ifndef LockerHPP
#include "Locker.hpp"
#endif // NOT LockerHPP

#endif // NOT LockerH
