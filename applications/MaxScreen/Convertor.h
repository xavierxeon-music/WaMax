#ifndef ConvertorH
#define ConvertorH

template <class IntClass>
struct Convertor
{
   union
   {
      IntClass data = 0;
      char bytes[sizeof(IntClass)];
   };
};

#endif // NOT ConvertorH
