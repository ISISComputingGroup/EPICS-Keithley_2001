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
        std::map<int, aSubOutputParameters> channel_output = asub_channel_output(prec);

        // Iterator over the map
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            int channel = set_double_value(it, channel_output);
        }
    }
    catch (const std::invalid_argument& e) {
        errlogSevPrintf(errlogMajor, "%s Invalid argument exception: %s", prec->name, e.what());
    }
    catch (const std::runtime_error& e) {
        errlogSevPrintf(errlogMajor, "%s Runtime Exception: %s", prec->name, e.what());
    }
    catch (...) {
        errlogSevPrintf(errlogMajor, "%s unknown exception", prec->name);
    }
    return 0;
}

// Parses the units from readings and sets the 
// unit to the respective channels.
// 
// Called from an aSub record.
long ParseReadingsForUnit(aSubRecord *prec) {
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
        std::map<int, aSubOutputParameters> channel_output = asub_channel_output(prec);

        // Iterator over the map
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            int channel = set_unit_value(it, channel_output);
        }
    }
    catch (const std::invalid_argument& e) {
        errlogSevPrintf(errlogMajor, "%s Invalid argument exception: %s", prec->name, e.what());
    }
    catch (const std::runtime_error& e) {
        errlogSevPrintf(errlogMajor, "%s Runtime Exception: %s", prec->name, e.what());
    }
    catch (...) {
        errlogSevPrintf(errlogMajor, "%s unknown exception", prec->name);
    }
    return 0;
}
