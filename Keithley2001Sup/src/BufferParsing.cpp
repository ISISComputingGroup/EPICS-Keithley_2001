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

#include "BufferParsing.h"

long ParseBufferReadings(aSubRecord *prec) {

	char(*a)[40] = static_cast<char(*)[40]>(prec->a);

	const int reading_values_count = 2;
	int i = 0;

	while (ParseReading(a[i]) > 0) {

		double value = ParseReading(a[i]);
		int channel = ParseChannel(a[i + 1]);
		std::cout << "Setting value " << value << "to channel " << channel << std::endl;

		switch (channel)
		{
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
	return 0; /* process output links */
}

long ParseSingleReading(aSubRecord *prec) {

	char(*a)[40] = static_cast<char(*)[40]>(prec->a);

	double value = ParseReading(a[0]);
	int channel = ParseChannel(a[1]);

	switch (channel)
	{
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
	return 0; /* process output links */
}

long ParseSingleUnit(aSubRecord *prec) {

	char(*a)[40] = static_cast<char(*)[40]>(prec->a);

	const char* unit = ParseUnit(a[0]);
	int channel = ParseChannel(a[1]);
	std::cout << "Setting Channel " << channel << "unit to " << unit << std::endl;

	switch (channel)
	{
	case 1:
		strcpy(static_cast<char*>(prec->vala), unit);
		break;
	case 2:
		strcpy(static_cast<char*>(prec->valb), unit);
		break;
	case 3:
		strcpy(static_cast<char*>(prec->valc), unit);
		break;
	case 4:
		strcpy(static_cast<char*>(prec->vald), unit);
		break;
	case 5:
		strcpy(static_cast<char*>(prec->vale), unit);
		break;
	case 6:
		strcpy(static_cast<char*>(prec->valf), unit);
		break;
	case 7:
		strcpy(static_cast<char*>(prec->valg), unit);
		break;
	case 8:
		strcpy(static_cast<char*>(prec->valh), unit);
		break;
	case 9:
		strcpy(static_cast<char*>(prec->vali), unit);
		break;
	case 10:
		strcpy(static_cast<char*>(prec->valj), unit);
		break;
	default:
		std::cout << "\nNo channel found for value " << channel << std::endl;
		break;
	}
	return 0; /* process output links */
}

double ParseReading(char reading[]) {
	return std::atof(reading);
}

int ParseChannel(char channel[]) {	
	return std::atoi(channel);
}

const char* ParseUnit(char reading[]) {
	return "VDC";
}
