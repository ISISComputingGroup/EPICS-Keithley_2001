#ifndef BufferParsing
#define BufferParsing

#include <aSubRecord.h>

#ifdef __cplusplus
extern "C" {
#endif
	// aSub functions
    long ParseBufferReadings(aSubRecord *prec);
	long ParseSingleReading(aSubRecord *prec);
	long ParseSingleUnit(aSubRecord *prec);

	// Helper functions
	double ParseReading(char reading[]);
	int ParseChannel(char channel[]);
	const char* ParseUnit(char reading[]);

#ifdef __cplusplus
}
#endif 

#endif// !BufferParsing
