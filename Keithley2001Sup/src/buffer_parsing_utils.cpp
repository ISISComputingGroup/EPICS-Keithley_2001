/* Helper functions*/
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <registryFunction.h>
#include <aSubRecord.h>
#include <menuFtype.h>
#include <epicsExport.h>
#include <menuFtype.h>
#include "epicsTypes.h"
#include <errlog.h>
#include <epicsString.h>
#include <string.h>
#include <stdlib.h>

#include "buffer_parsing_utils.h"

/**
 * Parses an array of strings to create a lookup table for readings by channel number.
 *
 * Args:
 *    input: Input aray from an aSubrecord.
 *    input_length: Max length of the input array.
 * Returns:
 *    map of channels as keys and readings as values.
 */
std::map<int, std::string> parse_input(void* input, epicsUInt32 input_length) {   
    std::vector<std::string> args;

    for (unsigned int i = 0; i < input_length; ++i) {
        std::string s(((epicsOldString*)(input))[i], sizeof(epicsOldString));
        args.push_back(s);
    }

    std::map<int, std::string> channels;
    unsigned int i = 0;

    while (i < args.size() - 1 && parse_channel(args[i]) != 0) {
        std::string reading(args[i]);
        std::string channel_string(args[i + 1]);
        int channel = parse_channel(channel_string);

        channels.insert(std::pair<int, std::string>(channel, reading));

        i += 2;
    }
    return channels;
}

/**
* Creates a lookup map to aSub record outputs 
*
* Args:
*    prec: aSubrecord.
* Returns:
*    map of channels as keys and aSub record output pointers as values.
*/
std::map<int, void*> asub_channel_output(aSubRecord *prec) {
    std::map<int, void*> channel_output;
    
    channel_output.insert(std::pair<int, void*>(1, prec->vala));
    channel_output.insert(std::pair<int, void*>(2, prec->valb));
    channel_output.insert(std::pair<int, void*>(3, prec->valc));
    channel_output.insert(std::pair<int, void*>(4, prec->vald));
    channel_output.insert(std::pair<int, void*>(5, prec->vale));
    channel_output.insert(std::pair<int, void*>(6, prec->valf));
    channel_output.insert(std::pair<int, void*>(7, prec->valg));
    channel_output.insert(std::pair<int, void*>(8, prec->valh));
    channel_output.insert(std::pair<int, void*>(9, prec->vali));
    channel_output.insert(std::pair<int, void*>(10, prec->valj));
    channel_output.insert(std::pair<int, void*>(11, prec->valk));
    channel_output.insert(std::pair<int, void*>(12, prec->vall));
    channel_output.insert(std::pair<int, void*>(13, prec->valm));
    channel_output.insert(std::pair<int, void*>(14, prec->valn));
    channel_output.insert(std::pair<int, void*>(15, prec->valo));
    channel_output.insert(std::pair<int, void*>(16, prec->valp));

    return channel_output;
}

/**
* Sets double values to output pointers of an aSub record.
*
* Args:
*    it: Iterator over a map of channel readings.
*    asub_outputs: Map of aSub output pointers.
* Returns:
*    long: -1 if succesfully sets the value, channel integer otherwise.
*/
int set_double_value(std::map<int, std::string>::iterator it, std::map<int, void*> asub_outputs) {
    // Accessing KEY from element pointed by it.
    int channel = it->first;

    // Accessing VALUE from element pointed by it.
    double value = parse_reading(it->second);

    // Set the value 
    double* channel_pt = static_cast<double*>(asub_outputs[channel]);
    if (channel_pt) {
        channel_pt[0] = value;
        return -1;
    }
    else {
        return channel;
    }
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
