#include <string.h>
#include <stdlib.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <menuFtype.h>
#include <errlog.h>
#include <epicsString.h>
#include <epicsExport.h>

#include <string>
#include <vector>
#include <iostream>

#include "buffer_parsing.h"
#include "buffer_parsing_utils.h"

// Parses the values from readings and sets the 
// value to the respective channels.
// 
// Called from an aSub record.
long ParseReadingsForValue(aSubRecord *prec) 
{
    std::vector<std::string> readings = device_readings(prec);

    const int reading_values_count = 2;
    unsigned int i = 0;

    while (parse_channel(readings[i + 1]) > 0 && i < prec->noa) {

        double value = parse_reading(readings[i]);
        int channel = parse_channel(readings[i + 1]);

        switch (channel) {
            case 1:
                static_cast<double*>(prec->vala)[0] = value;
                break;
            case 2:
                static_cast<double*>(prec->valb)[0] = value;
                break;
            case 3:
                static_cast<double*>(prec->valc)[0] = value;
                break;
            case 4:
                static_cast<double*>(prec->vald)[0] = value;
                break;
            case 5:
                static_cast<double*>(prec->vale)[0] = value;
                break;
            case 6:
                static_cast<double*>(prec->valf)[0] = value;
                break;
            case 7:
                static_cast<double*>(prec->valg)[0] = value;
                break;
            case 8:
                static_cast<double*>(prec->valh)[0] = value;
                break;
            case 9:
                static_cast<double*>(prec->vali)[0] = value;
                break;
            case 10:
                static_cast<double*>(prec->valj)[0] = value;
                break;
            default:
                std::cout << "\nNo channel found for value " << channel << std::endl;
                break;
        }
        i += reading_values_count;
    }
    return 0;
}

// Parses the units from readings and sets the 
// unit value to the respective channels.
// 
// Called from an aSub record.
long ParseReadingsForUnit(aSubRecord *prec) 
{
    std::vector<std::string> readings = device_readings(prec);

    const int reading_values_count = 2;
    unsigned int i = 0;

    while (parse_channel(readings[i + 1]) > 0 && i < prec->noa) {

        const char* unit = parse_unit(readings[i]);
        int channel = parse_channel(readings[i + 1]);

        switch (channel) {
        
        case 1:
            strcpy(*static_cast<epicsOldString*>(prec->vala), unit);
            break;
        case 2:
            strcpy(*static_cast<epicsOldString*>(prec->valb), unit);
            break;
        case 3:
            strcpy(*static_cast<epicsOldString*>(prec->valc), unit);
            break;
        case 4:
            strcpy(*static_cast<epicsOldString*>(prec->vald), unit);
            break;
        case 5:
            strcpy(*static_cast<epicsOldString*>(prec->vale), unit);
            break;
        case 6:
            strcpy(*static_cast<epicsOldString*>(prec->valf), unit);
            break;
        case 7:
            strcpy(*static_cast<epicsOldString*>(prec->valg), unit);
            break;
        case 8:
            strcpy(*static_cast<epicsOldString*>(prec->valh), unit);
            break;
        case 9:
            strcpy(*static_cast<epicsOldString*>(prec->vali), unit);
            break;
        case 10:
            strcpy(*static_cast<epicsOldString*>(prec->valj), unit);
            break;
        default:
            std::cout << "\nNo channel found for value " << channel << std::endl;
            break;
        }
        i += reading_values_count;
    }
    return 0;
}
