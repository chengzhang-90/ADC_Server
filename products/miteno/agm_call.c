#include "agm_call.h"
#include "udo.h"
#include "miteno.h"

AgmFunRegister astAgmFunRegister[] =
{
   {"adc_211003", adc_211003_main},
   {"adc_211004", adc_211004_main},
   {"adc_211005", adc_211005_main},
   {"adc_211006", adc_211006_main},
   {"adc_211007", adc_211007_main},
   {"adc_211008", adc_211008_main},
   {"adc_211009", adc_211009_main},
   {"adc_211010", adc_211010_main},
   {"adc_211011", adc_211011_main},
   {"adc_211012", adc_211012_main},
   {"adc_211013", adc_211013_main},
   {"adc_211014", adc_211014_main},
   {"adc_211015", adc_211015_main},
   {"adc_211016", adc_211016_main},
   {"adc_211017", adc_211017_main},
   {"adc_211018", adc_211018_main},
   {"adc_211019", adc_211019_main},
   {"adc_211022", adc_211022_main},
   {"adc_211023", adc_211023_main},
   {"adc_211024", adc_211024_main},
   {"adc_211025", adc_211025_main},
   {"adc_211026", adc_211026_main},
};

AgmFunManager stAgmFunManager =
{
   sizeof(astAgmFunRegister)/sizeof(AgmFunRegister), astAgmFunRegister
};
