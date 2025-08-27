#include "jit.wa.display.h"

#include <MaxPatcher.h>

JitDisplay::JitDisplay(const atoms& args)
   : object<JitDisplay>()
   , matrix_operator<>(true)
   , inputMatrix{this, "(matrix) Input", "matrix"}
   , outputMatrix{this, "(matrix) passthrough", "matrix"}
   , name{this, "name", "dummy"}
   , jitClassSetup{this, "jitclass_setup", Max::Patcher::minBind(this, &JitDisplay::jitClassSetupFunction)}
   , buffer()
   , sender(nullptr, "https://schweinesystem.ddns.net/flexdisplay")
{
}

template <typename matrix_type>
matrix_type JitDisplay::calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position)
{
   return matrix_type{};
}

pixel JitDisplay::calc_cell(pixel input, const matrix_info& info, matrix_coord& position)
{
   const int w = info.width();
   const int h = info.height();
   if (buffer.width() != w || buffer.height() != h)
   {
      buffer = QImage(w, h, QImage::Format_ARGB32);
   }

   const int x = position.x();
   const int y = position.y();

   const QRgb color = qRgb(input[red], input[green], input[blue]);
   QRgb* line = reinterpret_cast<QRgb*>(buffer.scanLine(y));
   line[x] = color;

   const pixel output = input;
   return output;
}

long JitDisplay::maxBangMessage(c74::max::t_object* x, long index, long count)
{
   minwrap<JitDisplay>* ob = (minwrap<JitDisplay>*)(x);
   ob->m_min_object.sendTexture();
   return 0;
}

void JitDisplay::sendTexture()
{
   const std::string target = name.get();
   cout << "send texture " << target << endl;
}

atoms JitDisplay::jitClassSetupFunction(const atoms& args, const int inlet)
{
   using namespace c74;

   // add mop
   auto mop = (max::t_jit_object*)max::jit_object_new(max::_jit_sym_jit_mop, 1, 1);
   max::jit_class_addadornment(this_jit_class, mop);

   // add methods
   max::jit_class_addmethod(this_jit_class, reinterpret_cast<max::method>(jit_matrix_calc<JitDisplay>), "matrix_calc", max::A_CANT, 0);
   max::jit_class_addmethod(this_jit_class, reinterpret_cast<max::method>(JitDisplay::maxBangMessage), "bang", A_DEFER_LOW, 0);

   return {};
}

MIN_EXTERNAL(JitDisplay);