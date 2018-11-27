/* Helper functions*/
#include <aSubRecord.h>
#include <iostream>

#include <string>
#include <vector>

#include "buffer_parsing_utils.h"

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
