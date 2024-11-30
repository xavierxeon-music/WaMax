#ifndef JitThingH
#define JitThingH

#include "jit.mo.common.h"
#include "jit.mo.singleton.h"
#include "noise1234.h" // https://github.com/stegu/perlin-noise

using namespace c74;
using namespace jit_mo;
using namespace c74::min;

using max::gensym;
using max::gettime;
using max::max_jit_obex_jitob_get;
using max::method;
using max::object_attr_getlong;
using max::outlet_float;
using max::t_atom;
using max::t_class;
using max::t_object;
using max::t_symbol;

void wa_jit_thing_update_anim(t_object* job, t_atom* a);
void max_wa_jit_thing_int(max_jit_wrapper* mob, long v);
void max_wa_jit_thing_bang(max_jit_wrapper* mob);
void max_wa_jit_thing_assist(void* x, void* b, long m, long a, char* s);

namespace timemodes
{
   static const symbol accum = "accum";
   static const symbol delta = "delta";
   static const symbol function = "function";
}; // namespace timemodes

#endif // NOT JitThingH
