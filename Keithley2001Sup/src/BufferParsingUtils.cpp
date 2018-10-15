/* Helper functions*/
#include <aSubRecord.h>

#include <string>
#include <vector>

std::vector<std::string> device_readings(aSubRecord *prec) 
{
	std::vector<std::string> readings;
	for (unsigned int i = 0; i < prec->noa; i++)
	{
		std::string reading((static_cast<epicsOldString*>(prec->a))[i], sizeof(epicsOldString));
		readings.push_back(reading);
	}
	return readings;
}

double ParseReading(std::string reading) 
{
	return std::atof(reading.c_str());
}

int ParseChannel(std::string channel) 
{
	return std::atoi(channel.c_str());
}

const char* ParseUnit(std::string reading) 
{
	return "VDC";
}
