#ifndef buffer_parsing_utils
#define buffer_parsing_utils

#include <aSubRecord.h>
#include "epicsTypes.h"
#include <menuFtype.h>
#include <map>

    struct aSubOutputParameters{

        void* outputPointer;
        menuFtype outputType;

        aSubOutputParameters() {}
        aSubOutputParameters(void* output_pointer, menuFtype output_type)
        : outputPointer(output_pointer), outputType(output_type) {}
    };
    
    std::map<int, std::string> parse_input(void* input, epicsUInt32 input_length);
    std::map<int, aSubOutputParameters> asub_channel_output(aSubRecord *prec);
    void set_double_value(std::map<int, std::string>::iterator it, std::map<int, aSubOutputParameters> asub_outputs);
    void set_unit_value(std::map<int, std::string>::iterator it, std::map<int, aSubOutputParameters> asub_outputs);
    double parse_reading(std::string reading);
    int parse_channel(std::string channel);
    std::string parse_reading_unit(std::string reading);



#endif
