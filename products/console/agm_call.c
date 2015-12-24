#include "agm_call.h"
#include "udo.h"
#include "console.h"

AgmFunRegister astAgmFunRegister[] =
{
   {"adc_203001", adc_203001_main},
};

AgmFunManager stAgmFunManager =
{
   sizeof(astAgmFunRegister)/sizeof(AgmFunRegister), astAgmFunRegister
};
