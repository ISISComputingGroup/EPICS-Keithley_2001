#include <stdlib.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include "buffer_parsing.h"

static long parse_buffer_readings(aSubRecord *prec) 
{
	return parse_buffer_readings_impl(prec);
}

epicsRegisterFunction(parse_buffer_readings); 
