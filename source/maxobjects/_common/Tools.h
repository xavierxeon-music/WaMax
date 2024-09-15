#ifndef ToolsH
#define ToolsH

namespace Tools
{
   class Range
   {
   public:
      Range(const double& min, const double& max);

   public:
      double clamp(const double& value) const;
      double operator()(const double& value) const; // same as clamp

      double factor(const double& value) const; // percentage between min and max
      double value(const double& factor) const; // value of percentage

   private:
      const double min;
      const double max;
      const double diff;
   };

   class Mapper
   {
   public:
      Mapper(const Range& input, const Range& output = Range(0.0, 1.0));

   public:
      double scale(const double& value) const;
      double operator()(const double& value) const; // same as scale

   private:
      const Range input;
      const Range output;
   };
} // namespace Tools

#ifndef ToolsHPP
#include "Tools.hpp"
#endif // NOT ToolsHPP

#endif // NOT ToolsH
