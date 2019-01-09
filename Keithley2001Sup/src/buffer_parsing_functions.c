#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include "buffer_parsing.h"

long parse_readings_for_value(aSubRecord *prec) {
    return ParseReadingsForValue(prec);
}

long parse_readings_for_unit(aSubRecord *prec) {
    return ParseReadingsForUnit(prec);
}

epicsRegisterFunction(parse_readings_for_value);
epicsRegisterFunction(parse_readings_for_unit);
