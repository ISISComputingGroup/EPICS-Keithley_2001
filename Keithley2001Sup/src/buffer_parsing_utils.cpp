/* Helper functions*/
#include <aSubRecord.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <menuFtype.h>
#include "epicsTypes.h"
#include <errlog.h>
#include <epicsString.h>

#include "buffer_parsing_utils.h"

std::map<int, std::string> parseInput(const char* input[], unsigned int input_length) {
    std::map<int, std::string> channels;
    unsigned int i = 0;

    while (i < input_length && std::atoi(input[i]) != 0) {
        try {
            std::string reading(input[i]);
            std::string channel_string(input[i + 1]);
            int channel = parse_channel(channel_string);

            channels.insert(std::pair<int, std::string>(channel, reading));
        }
        catch (std::out_of_range) {
            //errlogSevPrintf(errlogMajor, "Caught out of range error, device output string too short?");
            printf("Log this error");
        }
        i += 2;
    }
    return channels;
}

// Gets the readings from the aSub record input.
std::vector<std::string> device_readings(aSubRecord *prec) {
    std::vector<std::string> readings;
    for ( unsigned int i = 0; i < prec->noa; i++) {
        std::string reading((static_cast<epicsOldString*>(prec->a))[i], sizeof(epicsOldString));
        readings.push_back(reading);
    }
    return readings;
}

// Parses the channel number.
int parse_channel(std::string channel) {
    return std::atoi(channel.c_str());
}

// Parses the value.
double parse_reading(std::string reading) {
    return std::atof(reading.c_str());
}

// Parses the unit.
const char* parse_unit(std::string reading) {
    return "VDC";
}
