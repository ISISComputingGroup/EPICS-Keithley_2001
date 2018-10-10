program keithley_2001 ("P")

/*  This sequencer program runs the Keithley 2001.
 * 
 */ 
 
/* ==================== Sequencer compiler options =============================*/
 
/*option +r;*/
 
/* ==================== MACROS and includes =============================*/

#define MAX_INT_CHANNELS 10
#define NEXT 1
#define SENS1 1
#define FULL 0

%% #include "stdlib.h"
%% #include "stdio.h"
%% #include "string.h"
%% #include "dbDefs.h"
%% #include "seqPVmacros.h"

/* Include helper functions */
%% #include "KeithleyUtils.h"


/*=================== Variable declarations =========================*/

/* PVs set on startup */

int set_read_elements;
assign set_read_elements to "{P}ELEMENTS:SP.PROC";

int reset_device;
assign reset_device to "{P}RESET.PROC";

int reset_status_registers;
assign reset_status_registers to "{P}STAT:RESET.PROC";

int buffer_source = SENS1;
assign buffer_source to "{P}BUFF:SOURCE:SP.VAL";

int buffer_egroup = FULL;
assign buffer_egroup to "{P}BUFF:EGROUP:SP.VAL";

int set_register_status_buffer_full;
assign set_register_status_buffer_full to "{P}STAT:BUFF:FULL:SP.PROC";

int set_measure_summary_register_bit;
assign set_measure_summary_register_bit to "{P}STAT:MEAS:SUMM:SP.PROC";

int set_scan_count = 1;
assign set_scan_count to "{P}SCAN:COUNT:SP.VAL";

/* Channel PVs */

int active_channels[MAX_INT_CHANNELS];
assign active_channels to {
        "{P}CHAN:01:ACTIVE",
        "{P}CHAN:02:ACTIVE",
        "{P}CHAN:03:ACTIVE",
        "{P}CHAN:04:ACTIVE",
        "{P}CHAN:05:ACTIVE",
        "{P}CHAN:06:ACTIVE",
        "{P}CHAN:07:ACTIVE",
        "{P}CHAN:08:ACTIVE",
        "{P}CHAN:09:ACTIVE",
        "{P}CHAN:10:ACTIVE"
        };
monitor active_channels;

double channels_readings[MAX_INT_CHANNELS];
assign channels_readings to {
        "{P}CHAN:01:READ",
        "{P}CHAN:02:READ",
        "{P}CHAN:03:READ",
        "{P}CHAN:04:READ",
        "{P}CHAN:05:READ",
        "{P}CHAN:06:READ",
        "{P}CHAN:07:READ",
        "{P}CHAN:08:READ",
        "{P}CHAN:09:READ",
        "{P}CHAN:10:READ"
        };
monitor channels_readings;

string channel_units[MAX_INT_CHANNELS];
assign channel_units to {
        "{P}CHAN:01:UNIT",
        "{P}CHAN:02:UNIT",
        "{P}CHAN:03:UNIT",
        "{P}CHAN:04:UNIT",
        "{P}CHAN:05:UNIT",
        "{P}CHAN:06:UNIT",
        "{P}CHAN:07:UNIT",
        "{P}CHAN:08:UNIT",
        "{P}CHAN:09:UNIT",
        "{P}CHAN:10:UNIT"
        };
monitor channel_units;

/* Buffer PVs */

int clear_buffer;
assign clear_buffer to "{P}BUFF:CLEAR.PROC";

int set_buffer_mode;
assign set_buffer_mode to "{P}BUFF:MODE:SP";

int buffer_mode;
assign buffer_mode to "{P}BUFF:MODE";
monitor buffer_mode;

int buffer_size;
assign buffer_size to "{P}BUFF:SIZE:SP.VAL";

/* Single read PVS */

string set_single_channel_to_read;
assign set_single_channel_to_read to "{P}READ:CHAN:SP.VAL";

int read_single;
assign read_single to "{P}READ:SINGLE.PROC";

string single_channel_response;
assign single_channel_response to "{P}READ:SINGLE.VAL";
monitor single_channel_response;

/* Scan Pvs */

string set_channels_to_scan;
assign set_channels_to_scan to "{P}SCAN:CHAN:SP";

long measurement_scan_count_setpoint;
assign measurement_scan_count_setpoint to "{P}SCAN:MEAS:COUNT:SP.VAL";

int initialise_read;
assign initialise_read to "{P}SCAN:INIT.PROC";

int get_data;
assign get_data to "{P}SCAN:BUFF.PROC";

/* Scan and single read PVs */

double scan_speed;
assign scan_speed to "{P}SCAN:SPEED.VAL";
monitor scan_speed;

int scan_mode;
assign scan_mode to "{P}READ:MODE.VAL";

int number_of_active_channels;
assign number_of_active_channels to "{P}READ:_NUM_OF_CHANS.VAL";
monitor number_of_active_channels;

/* Non PV variables */

char *P;

int previous_channel;
int previous_scan_mode;
int previous_number_of_channels;

foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 
foreign CHAR;
%%int CHAR = sizeof(char);
foreign INT;
%%int INT = sizeof(int);

int *scanning_channels;
int single_channel;
string scan_channels_string;


/*=================== State Sets ===========================*/

ss running {
    state init {
        when() {
            P = macValueGet("P");
            pvPut(reset_device);
            pvPut(reset_status_registers);
            pvPut(clear_buffer);    
            
            pvPut(set_read_elements);
            pvPut(buffer_source);
            pvPut(buffer_egroup);
            
            pvPut(set_register_status_buffer_full);
            pvPut(set_measure_summary_register_bit);
            
            pvPut(set_scan_count);
            
            previous_channel = -1;
            previous_scan_mode = -1;
            previous_number_of_channels = -1;
        } state read_channel
    }
     
    state read_channel {
        
        when(number_of_active_channels == 0) {
            if (previous_scan_mode != 0) {
                scan_mode = 0;
                pvPut(scan_mode); 
                previous_scan_mode = scan_mode;
            }
        } state read_channel
        
        when(number_of_active_channels == 1) {
            if (previous_scan_mode != 1) {
                scan_mode = 1;
                pvPut(scan_mode); 
                previous_scan_mode = scan_mode;
            }       
            scanning_channels = malloc(sizeof(INT) * number_of_active_channels);
            
           
            find_active_channels(active_channels, number_of_active_channels, scanning_channels);            
            single_channel = scanning_channels[0];
            free(scanning_channels);
            
            if (single_channel != previous_channel){                
                sprintf(set_single_channel_to_read, "%d", single_channel);
                pvPut(set_single_channel_to_read);  
                previous_channel = single_channel;
            }
            pvPut(read_single);
        } state single_read_response
        
        when(number_of_active_channels > 1) {
            if (previous_scan_mode != 2) {
                scan_mode = 2;
                pvPut(scan_mode); 
                previous_scan_mode = scan_mode;
            }          
            if (previous_number_of_channels != number_of_active_channels){
                previous_number_of_channels = number_of_active_channels;
                
                buffer_size = number_of_active_channels;
                pvPut(buffer_size);
                
                measurement_scan_count_setpoint = number_of_active_channels;
                pvPut(measurement_scan_count_setpoint);
            }
            
            pvPut(clear_buffer);
            
            scanning_channels = malloc(sizeof(INT) * number_of_active_channels);
            memset(scanning_channels, 0, sizeof(scanning_channels));
            find_active_channels(active_channels, number_of_active_channels, scanning_channels);

            memset(scan_channels_string, 0, sizeof(scan_channels_string));
            generate_scan_channel_string(scanning_channels, number_of_active_channels, set_channels_to_scan);
            free(scanning_channels);
           
            pvPut(set_channels_to_scan);

        } state multi_read
    }
    
    state single_read_response{
        when(delay(scan_speed)) {                       
            sscanf(single_channel_response, "%lf%s", &channels_readings[single_channel - 1], &channel_units[single_channel - 1]);
            pvPut(channels_readings[single_channel - 1]);
            pvPut(channel_units[single_channel - 1]);              
        } state read_channel
    }
    
    state multi_read {
        
        when(buffer_mode != 1){
            set_buffer_mode = NEXT;
            pvPut(set_buffer_mode);
        } state multi_read
        
        when(buffer_mode == 1){
            pvPut(initialise_read);
        } state multi_read_response
    }
    
    state multi_read_response {
        when(scan_speed) {
            pvPut(get_data);
        } state read_channel
    }
}
