#ifndef buffer_parsing_utils
#define buffer_parsing_utils

#include <aSubRecord.h>
#include "epicsTypes.h"
#include <map>
    
    std::map<int, std::string> parse_input(void* input, epicsUInt32 input_length);
    std::map<int, void*> asub_channel_output(aSubRecord *prec);
    int set_double_value(std::map<int, std::string>::iterator it, std::map<int, void*> asub_outputs);
    double parse_reading(std::string reading);
    int parse_channel(std::string channel);
    const char* parse_unit(std::string reading);

#endif
