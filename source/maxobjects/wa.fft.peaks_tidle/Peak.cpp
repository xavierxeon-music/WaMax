#include "Peak.h"

bool Peak::operator<(const Peak& other) const
{
   return value < other.value;
}

bool Peak::operator>(const Peak& other) const
{
   return value > other.value;
}

std::ostream& operator<<(std::ostream& out, const Peak& peak)
{
   out << "[" << peak.value << " @ " << peak.index << "]";
   return out;
}
