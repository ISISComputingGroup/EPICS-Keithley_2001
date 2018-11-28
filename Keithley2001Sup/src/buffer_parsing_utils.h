#ifndef buffer_parsing_utils
#define buffer_parsing_utils

#include <aSubRecord.h>
#include <map>

    std::vector<std::string> device_readings(aSubRecord *prec);
    double parse_reading(std::string reading);
    int parse_channel(std::string channel);
    const char* parse_unit(std::string reading);
    std::map<int, std::string> parseInput(const char* input[], unsigned int input_length);

#endif
