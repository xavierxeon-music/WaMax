#ifndef LockerHPP
#define LockerHPP

#include "Locker.h"

template <size_t N>
constexpr CompileTimeString<N>::CompileTimeString(const char (&str)[N])
{
   std::copy_n(str, N, value);
}

//

template <CompileTimeString tag>
bool Locker<tag>::lock = false;

template <CompileTimeString tag>
Locker<tag>::Locker()
{
   lock = true;
}

template <CompileTimeString tag>
Locker<tag>::~Locker()
{
   lock = false;
}

template <CompileTimeString tag>
bool Locker<tag>::engaged()
{
   return lock;
}

#endif // NOT LockerHPP
