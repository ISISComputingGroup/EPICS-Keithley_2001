#include <registryFunction.h>
#include <epicsExport.h>
#include "aSubRecord.h"
#include "stdlib.h"
#include <stdio.h>
#include <menuFtype.h>

/*
*	Keithley 2001 Resistance Buffer Readings Parser
*
* 	Takes data from aSub record BUFF:READ, finds the Reading, Unit, and Channel value and 
*	puts it into the correct Channel. 
*
*	VALA (value A) = channel 01 reading = CHNL:01:READ etc
*
*
*/
static void assign_value_to_pv(double *val, epicsEnum16 ftv, double reading) {
	// Check that the input type is a double
	if(ftv != menuFtypeDOUBLE) {
		printf("\nIncorrect input type, cannot process this value");
	}
	else {
		((double *)val)[0] = reading;
	}
}
	
static long parse_buffer_readings(aSubRecord *prec, long array_offset) {
	
	/* 	'array_offset' refers to the index of the buffer array. This value will be 0,1 or 2, allowing 
	* 	this script to parse the unit, timestamp and reading value depending on the value of array_offset.
	* 	a[i] = channel reading
    *   a[i+1] = channel unit
	* 	a[i+2] = channel 
	*/
	
	double *a;
	// We have 4 values per channel - reading, unit, timestamp and channel
	const int channel_values_count = 4;
	unsigned long i = 0;
	// prec = INPA from keithley2001.db, TRAC:DATA? - a waveform PV
    prec->pact = 1;
	
    a = (double *)prec->a;	
	
	while((a[i] > 0) && (i < (prec->noa)-channel_values_count)) {

		int channel = (int)(a[i+2]+0.5); // protection against double to int rounding errors		
		double value = a[i+array_offset];
		
		// Find the channel and add reading value to correct channel PV
		switch(channel) {
			case 1:
				assign_value_to_pv(prec->vala, prec->ftva, value);
				break;
			case 2:
				assign_value_to_pv(prec->valb, prec->ftva, value);
				break;
			case 3:
				assign_value_to_pv(prec->valc, prec->ftva, value);
				break;
			case 4:
				assign_value_to_pv(prec->vald, prec->ftva, value);
				break;
			case 5:
				assign_value_to_pv(prec->vale, prec->ftva, value);
				break;
			case 6:
				assign_value_to_pv(prec->valf, prec->ftva, value);
				break;
			case 7:
				assign_value_to_pv(prec->valg, prec->ftva, value);
				break;
			case 8:
				assign_value_to_pv(prec->valh, prec->ftva, value);
				break;
			case 9:
				assign_value_to_pv(prec->vali, prec->ftva, value);
				break;
			case 10:
				assign_value_to_pv(prec->valj, prec->ftva, value);
				break;
		}		
		i=i+channel_values_count;
    }
    prec->pact = 0;
    return 0;
}

static long parse_channel_readings(aSubRecord *prec) {
	return parse_buffer_readings(prec, 0);
}

static long parse_channel_units(aSubRecord *prec) {
	return parse_buffer_readings(prec, 1);
}

static long parse_channel_timestamps(aSubRecord *prec) {
	return parse_buffer_readings(prec, 2);
}

epicsRegisterFunction(parse_channel_readings);
epicsRegisterFunction(parse_channel_units);
epicsRegisterFunction(parse_channel_timestamps);
