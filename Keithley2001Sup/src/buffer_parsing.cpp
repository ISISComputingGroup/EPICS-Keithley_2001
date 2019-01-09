#include <string.h>
#include <stdlib.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <menuFtype.h>
#include <errlog.h>
#include <epicsString.h>
#include <epicsExport.h>
#include "epicsTypes.h"

#include <string>
#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>

#include "buffer_parsing.h"
#include "buffer_parsing_utils.h"
#include <boost/function.hpp>

/**
* Base function used to parse readings.
* Called from an aSub record.
*
* Args:
*    prec: Pointer to an aSub record.
*    Parser: Parsing function to parse the readings.
* Returns:
*    long: 0 if succesfully.
*/
static long aSubParser(aSubRecord *prec, 
    boost::function<void (std::map<int, std::string>::iterator, std::map<int, aSubOutputParameters>)> Parser) {
    if (prec->fta != menuFtypeSTRING)
    {
        errlogSevPrintf(errlogMajor, "%s incorrect input argument type A", prec->name);
        return 1;
    }
    try {
        // Create channel lookup maps
        std::map<int, std::string> channel_readings = parse_input(prec->a, prec->noa);
        if (channel_readings.empty()) {
            errlogSevPrintf(errlogInfo, "Buffer readings contain no channel information.");
            return 2;
        }
        std::map<int, aSubOutputParameters> channel_outputs = asub_channel_output(prec);

        // Iterator over the map
        for (std::map<int, std::string>::iterator it = channel_readings.begin();
            it != channel_readings.end();
            ++it) {
            Parser(it, channel_outputs);
        }
    }
    catch (std::invalid_argument& e) {
        errlogSevPrintf(errlogMajor, "%s Invalid argument exception: %s", prec->name, e.what());
        return 3;
    }
    catch (std::logic_error& e) {
        errlogSevPrintf(errlogMajor, "%s Logic Error Exception: %s", prec->name, e.what());
        return 4;
    }
    catch (std::exception& e) {
        errlogSevPrintf(errlogMajor, "%s Exception: %s", prec->name, e.what());
        return 5;
    }
    catch (...) {
        errlogSevPrintf(errlogMajor, "%s unknown exception", prec->name);
        return 6;
    }
    return 0;
}

// Parses the values from readings and sets the 
// value to the respective channels.
// 
// Called from an aSub record.
long ParseReadingsForValue(aSubRecord *prec) {
    return aSubParser(prec, set_double_value);
}

// Parses the units from readings and sets the 
// unit to the respective channels.
// 
// Called from an aSub record.
long ParseReadingsForUnit(aSubRecord *prec) {
    return aSubParser(prec, set_unit_value);
}
