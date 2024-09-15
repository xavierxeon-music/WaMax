#ifndef SpatialFunctionH
#define SpatialFunctionH

namespace Spatial
{
   class Function
   {
   public:
      Function(const double& az, const double& el, bool left);

   public:
      double value(int index) const;

   private:
      double max;
      double peak;
      double start;
      double end;
   };
} // namespace Spatial

#endif // NOT SpatialFunctionH
