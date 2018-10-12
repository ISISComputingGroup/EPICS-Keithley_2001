#include <stdlib.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include "BufferParsing.h"

static long parse_buffer_readings(aSubRecord *prec) {
	return ParseBufferReadings(prec);
}

static long parse_single_reading(aSubRecord *prec) {
	return ParseSingleReading(prec);
}

static long parse_single_unit(aSubRecord *prec) {
	return ParseSingleUnit(prec);
}

epicsRegisterFunction(parse_buffer_readings); 
epicsRegisterFunction(parse_single_reading);
epicsRegisterFunction(parse_single_unit);
