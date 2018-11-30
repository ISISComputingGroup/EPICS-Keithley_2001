/* Helper functions*/
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

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
std::map<int, aSubOutputParameters> asub_channel_output(aSubRecord *prec) {
    std::map<int, aSubOutputParameters> channel_output;
    
    channel_output.insert(std::pair<int, aSubOutputParameters>(1,   aSubOutputParameters(prec->vala, prec->ftva)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(2,   aSubOutputParameters(prec->valb, prec->ftvb)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(3,   aSubOutputParameters(prec->valc, prec->ftvc)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(4,   aSubOutputParameters(prec->vald, prec->ftvd)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(5,   aSubOutputParameters(prec->vale, prec->ftve)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(6,   aSubOutputParameters(prec->valf, prec->ftvf)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(7,   aSubOutputParameters(prec->valg, prec->ftvg)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(8,   aSubOutputParameters(prec->valh, prec->ftvh)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(9,   aSubOutputParameters(prec->vali, prec->ftvi)));
    channel_output.insert(std::pair<int, aSubOutputParameters>(10,  aSubOutputParameters(prec->valj, prec->ftvj)));

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
void set_double_value(std::map<int, std::string>::iterator it, std::map<int, aSubOutputParameters> asub_outputs) {
    // Accessing KEY from element pointed by it.
    int channel = it->first;

    // Accessing VALUE from element pointed by it.
    double value = parse_reading(it->second);
    
    // Try to access the channel's output parameters from the map.
    try {
        aSubOutputParameters outputParameters = asub_outputs.at(channel);
        double* channel_value_pointer = static_cast<double*>(outputParameters.outputPointer);
        
        if (channel_value_pointer) {
            // Check channel output type
            if (outputParameters.outputType != menuFtypeDOUBLE) {
                std::stringstream error_string;
                error_string << "Incorrect output type " << outputParameters.outputPointer << " for channel " << channel << std::endl;
                throw std::invalid_argument(error_string.str());
            }
            channel_value_pointer[0] = value;
        }
        else {
            std::stringstream error_string;
            error_string << "Pointer for channel " << channel << " is NULL" << std::endl;
            throw std::logic_error(error_string.str());
        }
    }
    catch(std::out_of_range) {
        std::stringstream error_string;
        error_string << "Channel " << channel << " is not a valid output channel." << std::endl;
        throw std::invalid_argument(error_string.str());
    }
}

/**
* Sets double values to output pointers of an aSub record.
*
* Args:
*    it: Iterator over a map of channel readings.
*    asub_outputs: Map of aSub output pointers.
* Returns:
*    int: 0 if succesfully.
*/
void set_unit_value(std::map<int, std::string>::iterator it, std::map<int, aSubOutputParameters> asub_outputs) {
    // Accessing KEY from element pointed by it.
    int channel = it->first;

    // Accessing UNIT from element pointed by it.
    std::string unit = parse_reading_unit(it->second);

    // Try to access the channel's output parameters from the map.
    try {
        aSubOutputParameters outputParameters = asub_outputs.at(channel);
        epicsOldString* channel_value_pointer = static_cast<epicsOldString*>(outputParameters.outputPointer);

        if (channel_value_pointer) {
            // Check channel output type
            if (outputParameters.outputType != menuFtypeSTRING) {
                std::stringstream error_string;
                error_string << "Incorrect output type " << outputParameters.outputPointer << " for channel " << channel << std::endl;
                throw std::invalid_argument(error_string.str());
            }
            strcpy(*channel_value_pointer, unit.c_str());
        }
        else {
            std::stringstream error_string;
            error_string << "Pointer for channel " << channel << " is NULL" << std::endl;
            throw std::logic_error(error_string.str());
        }
    }
    catch (std::out_of_range) {
        std::stringstream error_string;
        error_string << "Channel " << channel << " is not a valid output channel." << std::endl;
        throw std::invalid_argument(error_string.str());
    }
}

/**
* Parses the channel.
*
* Args:
*    reading: String to be parsed. Expected format %iINTCHAN where %i is a zero padded integer
*             between 1 and 10.
* Returns:
*    int: Value of underlying c string as an integer.
*/
int parse_channel(std::string reading) {
    return std::atoi(reading.c_str());
}

/**
* Parses the reading out of a reading.
*
* Args:
*    reading: String to be parsed. 
* Returns:
*    double: Value of the double reading.
*/
double parse_reading(std::string reading) {
    std::stringstream stream(reading);
    double value;
    stream >> value;
    return value;
}

/**
* Parses the unit out of a reading.
*
* Args:
*    reading: String to be parsed.
* Returns:
*    std::string: Unit of the reading.
*/
std::string parse_reading_unit(std::string reading) {
    std::stringstream stream(reading);
    double value;
    std::string unit;

    stream >> value;
    stream >> unit;

    return unit;
}
