#include <stdlib.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include "BufferParsing.h"

static long parse_readings_for_value(aSubRecord *prec) {
	return ParseReadingsForValue(prec);
}

static long parse_readings_for_unit(aSubRecord *prec) {
	return ParseReadingsForUnit(prec);
}

epicsRegisterFunction(parse_readings_for_value);
epicsRegisterFunction(parse_readings_for_unit);
