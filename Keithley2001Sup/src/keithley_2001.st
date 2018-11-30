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
%% #include "stdlib.h"
%% #include "sequencer_utils.h"
%% #include "errlog.h"

/*=================== Variable declarations =========================*/

/* PVs set on startup */

int set_read_elements;
assign set_read_elements to "{P}ELEMENTS:SP.PROC";

int reset_device;
assign reset_device to "{P}RESET.PROC";

int reset_status_registers;
assign reset_status_registers to "{P}STAT:RESET.PROC";

int buffer_source = SENSOR;
assign buffer_source to "{P}BUFF:SOURCE:SP.VAL";

int buffer_read_elements = FULL;
assign buffer_read_elements to "{P}BUFF:EGROUP:SP.VAL";

int set_register_status_buffer_full;
assign set_register_status_buffer_full to "{P}STAT:BUFF:FULL:SP.PROC";

int set_measure_summary_register_bit;
assign set_measure_summary_register_bit to "{P}STAT:MEAS:SUMM:SP.PROC";

int set_scan_count = 1;
assign set_scan_count to "{P}SCAN:COUNT:SP.VAL";

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

int clear_buffer;
assign clear_buffer to "{P}BUFF:CLEAR.PROC";

int set_buffer_mode;
assign set_buffer_mode to "{P}BUFF:MODE:SP.VAL";

int buffer_mode;
assign buffer_mode to "{P}BUFF:MODE";

int buffer_size;
assign buffer_size to "{P}BUFF:SIZE:SP.VAL";

/* Single read PVS */

string set_single_channel_to_read;
assign set_single_channel_to_read to "{P}READ:CHAN:SP.VAL";

int read_single;
assign read_single to "{P}READINGS:SINGLE.PROC";

/* Scan Pvs */

string set_channels_to_scan;
assign set_channels_to_scan to "{P}SCAN:CHAN:SP";

long measurement_scan_count_setpoint;
assign measurement_scan_count_setpoint to "{P}SCAN:MEAS:COUNT:SP.VAL";

int start_scan;
assign start_scan to "{P}SCAN:START.PROC";

int get_buffer_data;
assign get_buffer_data to "{P}READINGS:BUFF.PROC";

/* Parse readings */

int parse_readings_for_value;
assign parse_readings_for_value to "{P}PARSE:VALUE.PROC";

int parse_readings_for_unit;
assign parse_readings_for_unit to "{P}PARSE:UNIT.PROC";


/* Scan and single read PVs */

double scan_delay;
assign scan_delay to "{P}SCAN:DELAY.VAL";
monitor scan_delay;

/* Error handling Pv */

int error;
assign error to "{P}ERROR:RAW.PROC";

int clear_error_flag;
assign clear_error_flag to "{P}ERROR:CLEAR:FLAG.VAL";

int clear_error;
assign clear_error to "{P}ERROR:CLEAR.PROC";


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
        } state check_for_errors
    }
    
    state check_for_errors {
        when () {
            pvGet(clear_error_flag);
            pvPut(error);
        } state clear_errors
    }
    
    state clear_errors {
    
        when(clear_error_flag == 1) {
            pvPut(clear_error);
            clear_error_flag = 0;
            pvPut(clear_error_flag);      
        } state prepare_to_read
        
        when(clear_error_flag == 0) {
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
        } state check_for_errors
        
        when(number_of_active_channels == 1) {   	
            memset(scanning_channels, 0, sizeof(scanning_channels));     
            active_channels_found = find_active_channels(active_channels, scanning_channels);            
        } state check_active_channels_are_found
        
        when(number_of_active_channels > 1) {          
            memset(scanning_channels, 0, sizeof(scanning_channels));       
            active_channels_found = find_active_channels(active_channels, scanning_channels);    
        } state check_active_channels_are_found
    }

	state check_active_channels_are_found {
		when(active_channels_found != 0) {
			%% errlogSevPrintf(errlogMajor, "Active channels not found");
		} state check_for_errors

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
                
                buffer_size = number_of_active_channels;
                pvPut(buffer_size);
                
                measurement_scan_count_setpoint = number_of_active_channels;
                pvPut(measurement_scan_count_setpoint);
            }
            
            pvPut(clear_buffer);

			generate_scan_channel_string(scanning_channels, number_of_active_channels, set_channels_to_scan);
            pvPut(set_channels_to_scan);    

            set_buffer_mode = 1;
            pvPut(set_buffer_mode);   
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
        } state check_for_errors
    }
}
