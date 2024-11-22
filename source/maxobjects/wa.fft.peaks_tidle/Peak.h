#ifndef PeakH
#define PeakH

#include <iostream>
#include <vector>

struct Peak
{
   int index = 0;
   double value = 0.0;

   using List = std::vector<Peak>;
   friend std::ostream& operator<<(std::ostream& out, const Peak& peak);

   bool operator<(const Peak& other) const;
   bool operator>(const Peak& other) const;
};

std::ostream& operator<<(std::ostream& out, const Peak& peak);

#endif // NOT PeakH
