program keithley_2001 ("P")

/*  This sequencer program for an IOC to control the Keithley 2001.
 *  It is designed to read from both a single channel or scan 
 *  on more than one channel. 
 *
 *  It only takes measurement of DC voltage.
 */ 
 
/* ==================== Sequencer compiler options =============================*/
 
option +r;
 
/* ==================== MACROS and includes =============================*/


#include "definitions.h"
#include "seqPVmacros.h"
%% #include "stdlib.h"
%% #include "sequencer_utils.h"
%% #include "errlog.h"

/*=================== Variable declarations =========================*/

/* PVs set on startup */

PV(int, set_read_elements, "{P}ELEMENTS:SP.PROC", NoMon);

PV(int, reset_device, "{P}RESET.PROC", NoMon);
PV(int, reset_status_registers, "{P}STAT:RESET.PROC", NoMon);
PV(int, set_register_status_buffer_full, "{P}STAT:BUFF:FULL:SP.PROC", NoMon);
PV(int, set_measure_summary_register_bit, "{P}STAT:MEAS:SUMM:SP.PROC", NoMon);
PV(int, buffer_source, "{P}BUFF:SOURCE:SP.VAL", NoMon);
PV(int, buffer_read_elements, "{P}BUFF:EGROUP:SP.VAL", NoMon);
PV(int, set_scan_count, "{P}SCAN:COUNT:SP.VAL", NoMon);

/* Channel PVs */

int active_channels[MAX_NUMBER_OF_CHANNELS];
assign active_channels to {
        "{P}CHAN:01:ACTIVE.VAL",
        "{P}CHAN:02:ACTIVE.VAL",
        "{P}CHAN:03:ACTIVE.VAL",
        "{P}CHAN:04:ACTIVE.VAL",
        "{P}CHAN:05:ACTIVE.VAL",
        "{P}CHAN:06:ACTIVE.VAL",
        "{P}CHAN:07:ACTIVE.VAL",
        "{P}CHAN:08:ACTIVE.VAL",
        "{P}CHAN:09:ACTIVE.VAL",
        "{P}CHAN:10:ACTIVE.VAL"
        };

/* Buffer PVs */

PV(int, clear_buffer, "{P}BUFF:CLEAR.PROC", NoMon);
PV(int, set_buffer_mode, "{P}BUFF:MODE:SP.VAL", NoMon);
PV(int, buffer_mode, "{P}BUFF:MODE.VAL", NoMon);
PV(int, buffer_size, "{P}BUFF:SIZE:SP.VAL", NoMon);

/* Single read PVS */

PV(string, set_single_channel_to_read, "{P}READ:CHAN:SP.VAL", NoMon);
PV(int, read_single, "{P}READINGS:SINGLE.PROC", NoMon);

/* Scan Pvs */

PV(string, set_channels_to_scan, "{P}SCAN:CHAN:SP", NoMon);
PV(long, measurement_scan_count_setpoint, "{P}SCAN:MEAS:COUNT:SP.VAL", NoMon);
PV(int, start_scan, "{P}SCAN:START.PROC", NoMon);
PV(int, get_buffer_data, "{P}READINGS:BUFF.PROC", NoMon);


/* Parse readings */

PV(int, parse_readings_for_value, "{P}PARSE:VALUE.PROC", NoMon);
PV(int, parse_readings_for_unit, "{P}PARSE:UNIT.PROC", NoMon);


/* Scan and single read PVs */

PV(double, scan_delay, "{P}SCAN:DELAY.VAL", Monitor);

/* Error handling Pv */

PV(int, error, "{P}ERROR:RAW.PROC", NoMon);

/* Reset IOC PV */
PV(int, reset_flag, "{P}RESET:FLAG.VAL", NoMon);


/* Non PV variables */

char *P;
int previous_channel;
int previous_number_of_channels;
int single_channel;
int scanning_channels[MAX_NUMBER_OF_CHANNELS] = {0,0,0,0,0,0,0,0,0,0};  
int number_of_active_channels;
int i = 0;
int active_channels_found;


/*=================== State Sets ===========================*/

ss running {

    state init {
        when() {
            buffer_source = SENSOR;
            buffer_read_elements = FULL;
            set_scan_count = 1;

            P = macValueGet("P");
            pvPut(reset_device);
            
            pvPut(reset_status_registers);
            pvPut(set_register_status_buffer_full);
            pvPut(set_measure_summary_register_bit);

            pvPut(set_read_elements);
            pvPut(buffer_read_elements); 
            pvPut(buffer_source);
            pvPut(set_scan_count);

            previous_channel = -1;
            previous_number_of_channels = -1;
        } state check_for_errors_and_if_to_reset
    }
    
    state check_for_errors_and_if_to_reset {
        when () {
            pvGet(reset_flag);
            pvPut(error);
        } state reset_ioc
    }
    
    state reset_ioc {
    
        when(reset_flag == 1) {
			PVPUT(reset_flag, 0);    
        } state init
        
        when(reset_flag == 0) {
        } state prepare_to_read
    }
   
    state prepare_to_read {
        when() {
            for (i = 0; i < MAX_NUMBER_OF_CHANNELS; i++){
                pvGet(active_channels[i]);
            }
            number_of_active_channels = sum_of_active_channels(active_channels);
            pvGet(buffer_mode);
        } state reading_setup
    }
 
    state reading_setup {
        when(number_of_active_channels == 0) {
        } state check_for_errors_and_if_to_reset
        
        when(number_of_active_channels == 1) {   	
            memset(scanning_channels, 0, sizeof(scanning_channels));     
            active_channels_found = find_active_channels(active_channels, scanning_channels);            
        } state check_if_active_channels_are_found
        
        when(number_of_active_channels > 1) {          
            memset(scanning_channels, 0, sizeof(scanning_channels));       
            active_channels_found = find_active_channels(active_channels, scanning_channels);    
        } state check_if_active_channels_are_found
    }

	state check_if_active_channels_are_found {
		when(active_channels_found != 0) {
			%% errlogSevPrintf(errlogMajor, "Active channels not found");
		} state check_for_errors_and_if_to_reset

		when(active_channels_found == 0 && number_of_active_channels == 1){
		} state single_shot

		when(active_channels_found == 0 && number_of_active_channels > 1){
		} state scanning
	}
    
    state single_shot {
        when(delay(scan_delay)) { 
			single_channel = scanning_channels[0];
			            
            if (single_channel != previous_channel){   
                sprintf(set_single_channel_to_read, "%d", single_channel);  
                pvPut(set_single_channel_to_read);
                previous_channel = single_channel;
            }
            pvPut(read_single);
        } state parse_readings
    }    
    
    state scanning {
        when(delay(scan_delay)) {
			if (previous_number_of_channels != number_of_active_channels){
                previous_number_of_channels = number_of_active_channels;
                
				PVPUT(buffer_size, number_of_active_channels);
                PVPUT(measurement_scan_count_setpoint, number_of_active_channels);
            }
            pvPut(clear_buffer);

			generate_scan_channel_string(scanning_channels, number_of_active_channels, set_channels_to_scan);
            pvPut(set_channels_to_scan);    

            PVPUT(set_buffer_mode, NEXT);   
            pvPut(start_scan);
        } state scanning_response
    }
    
    state scanning_response {
        when() {
            pvPut(get_buffer_data);
        } state parse_readings
    }

    state parse_readings {
        when() {
            pvPut(parse_readings_for_value);
            pvPut(parse_readings_for_unit);
        } state check_for_errors_and_if_to_reset
    }
}
