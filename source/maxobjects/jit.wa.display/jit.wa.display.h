#ifndef DisplayH
#define DisplayH

#include "c74_min.h"
using namespace c74::min;
#include <CommonQt.h>

#include <QImage>

#include <RestApiBlocking.h>

class JitDisplay : public object<JitDisplay>, public matrix_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   JitDisplay(const atoms& args = {});

public:
   template <typename matrix_type>
   matrix_type calc_cell(matrix_type input, const matrix_info& info, matrix_coord& position);

   pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position);

private:
   static long maxBangMessage(c74::max::t_object* x, long index, long count);
   void sendTexture();

   atoms jitClassSetupFunction(const atoms& args, const int inlet);

private:
   inlet<> inputMatrix;
   outlet<> outputMatrix;

   attribute<symbol> name;
   message<> jitClassSetup;

   QImage buffer;
   RestApi::Blocking sender;
};

#endif // DisplayH
