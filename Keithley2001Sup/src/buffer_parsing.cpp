#include <string.h>
#include <stdlib.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <menuFtype.h>
#include <errlog.h>
#include <epicsString.h>
#include <epicsExport.h>

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "buffer_parsing.h"

long parse_buffer_readings_impl(aSubRecord *prec) 
{
    /* Returns the first input value back */
    prec->vala = prec->a;
    return 0; /* process output links */
}
