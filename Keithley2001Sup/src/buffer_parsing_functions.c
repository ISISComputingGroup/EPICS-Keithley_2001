#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include "buffer_parsing.h"

static long parse_readings_for_value(aSubRecord *prec) {
    return ParseReadingsForValue(prec);
}

epicsRegisterFunction(parse_readings_for_value);
