#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "adc_syserr.h"

#ifndef _ADC_CONSOLE_H_
#define _ADC_CONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif

     int adc_203001_main( UDP epstInList, UDP opstOutList );

#ifdef __cplusplus
}
#endif

#endif 
