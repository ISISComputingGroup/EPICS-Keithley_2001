#ifndef buffer_parsing_utils
#define buffer_parsing_utils

#include <aSubRecord.h>

    std::vector<std::string> device_readings(aSubRecord *prec);
    double parse_reading(std::string reading);
    int parse_channel(std::string channel);
    const char* parse_unit(std::string reading);

#endif
