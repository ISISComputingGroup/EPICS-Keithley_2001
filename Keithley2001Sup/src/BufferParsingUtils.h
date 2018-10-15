#ifndef BufferParsingUtils
#define BufferParsingUtils

#include <aSubRecord.h>

std::vector<std::string> device_readings(aSubRecord *prec);
double ParseReading(std::string reading);
int ParseChannel(std::string channel);
const char* ParseUnit(std::string reading);

#endif
