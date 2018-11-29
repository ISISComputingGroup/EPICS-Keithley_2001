#include <string.h>
#include <stdlib.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <menuFtype.h>
#include <errlog.h>
#include <epicsString.h>
#include <epicsExport.h>

#include <string>
#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>

#include "buffer_parsing.h"
#include "buffer_parsing_utils.h"


// Parses the values from readings and sets the 
// value to the respective channels.
// 
// Called from an aSub record.
long ParseReadingsForValue(aSubRecord *prec) {
    if (prec->fta != menuFtypeSTRING)
    {
        errlogSevPrintf(errlogMajor, "%s incorrect input argument type A", prec->name);
        return 1;
    }
    try {
        // Create channel lookup maps
        std::map<int, std::string> channel_readings = parse_input(prec->a, prec->noa);
        if (channel_readings.empty()) {
            errlogSevPrintf(errlogMajor, "Buffer readings contain no channel information.");
            return 2;
        }
        std::map<int, void*> channel_output = asub_channel_output(prec);

        // Iterator over the map
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end(); 
            ++it) {
            int channel = set_double_value(it, channel_output);
            if (channel > -1) {
                errlogSevPrintf(errlogMajor, "Channel %d is not avalable", channel);
                return 3;
            }
        }
    }
    catch (std::out_of_range) {
        errlogSevPrintf(errlogMajor, "Caught out of range error");
    }
    catch (const std::exception& e) {
        errlogSevPrintf(errlogMajor, "%s exception %s", prec->name, e.what());
    }
    catch (...) {
    errlogSevPrintf(errlogMajor, "%s unknown exception", prec->name);
    }
    return 0;
}
