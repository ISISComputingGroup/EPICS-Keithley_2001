program keithley_2001 ("P")

/*  This sequencer program runs the Keithley 2001.
 * 
 */ 
 
/* ==================== Sequencer compiler options =============================*/
 
option +r;
 
/* ==================== MACROS and includes =============================*/

#define MAX_NUMBER_OF_CHANNELS 10
#define NEXT 1
#define SENS1 1
#define FULL 0
#define ACTIVE 1

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

double channels_readings[MAX_NUMBER_OF_CHANNELS];
assign channels_readings to {
        "{P}CHAN:01:READ.VAL",
        "{P}CHAN:02:READ.VAL",
        "{P}CHAN:03:READ.VAL",
        "{P}CHAN:04:READ.VAL",
        "{P}CHAN:05:READ.VAL",
        "{P}CHAN:06:READ.VAL",
        "{P}CHAN:07:READ.VAL",
        "{P}CHAN:08:READ.VAL",
        "{P}CHAN:09:READ.VAL",
        "{P}CHAN:10:READ.VAL"
        };

string channel_units[MAX_NUMBER_OF_CHANNELS];
assign channel_units to {
        "{P}CHAN:01:UNIT.VAL",
        "{P}CHAN:02:UNIT.VAL",
        "{P}CHAN:03:UNIT.VAL",
        "{P}CHAN:04:UNIT.VAL",
        "{P}CHAN:05:UNIT.VAL",
        "{P}CHAN:06:UNIT.VAL",
        "{P}CHAN:07:UNIT.VAL",
        "{P}CHAN:08:UNIT.VAL",
        "{P}CHAN:09:UNIT.VAL",
        "{P}CHAN:10:UNIT.VAL"
        };

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

/* Non PV variables */

char *P;

int previous_channel;
int previous_scan_mode;
int previous_number_of_channels;

foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 
foreign INT;
%%int INT = sizeof(int);

int single_channel;
int scanning_channels[MAX_NUMBER_OF_CHANNELS] = {0,0,0,0,0,0,0,0,0,0};     
int number_of_active_channels;
int i = 0;


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
        } state prepare_to_read
    }
    
    state prepare_to_read {
        when() {
            puts("Entering preparing to read\n");
            puts("The active channels are: \n");
            for (i = 0; i < MAX_NUMBER_OF_CHANNELS; i++){
                pvGet(active_channels[i]);
                printf("Channel %i: %i \n",i+1, active_channels[i]);
            }

            number_of_active_channels = get_number_of_active_channels(active_channels);
            printf("The number of active channels is: %i\n", number_of_active_channels);

            puts("Leaving preparing to read\n");
        } state read_channel
    }
 
    state read_channel {
        
        when(number_of_active_channels == 0) {
            if (previous_scan_mode != 0) {
                scan_mode = 0;
                pvPut(scan_mode); 
                previous_scan_mode = scan_mode;
            }

        } state prepare_to_read
        
        when(number_of_active_channels == 1) {
        puts("ENTERING SINGLE READ MODE\n");
            if (previous_scan_mode != 1) {
                scan_mode = 1;
                pvPut(scan_mode); 
                previous_scan_mode = scan_mode;
            }       
            memset(scanning_channels, 0, sizeof(scanning_channels));     
            find_active_channels(active_channels, scanning_channels);
            single_channel = scanning_channels[0];
            printf("The channel being read is: %i\n", single_channel);
            
            if (single_channel != previous_channel){   
                sprintf(set_single_channel_to_read, "%d", single_channel);  
                pvPut(set_single_channel_to_read);
                previous_channel = single_channel;
            }
            pvPut(read_single);
            puts("LEAVING SINGLE READ MODE\n");
        } state single_read_response
        
        when(number_of_active_channels > 1) {
        puts("ENTERING MULTI READ MODE\n");
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
            
            memset(scanning_channels, 0, sizeof(scanning_channels));       
            find_active_channels(active_channels, scanning_channels);

            generate_scan_channel_string(scanning_channels, number_of_active_channels, set_channels_to_scan);
           
            pvPut(set_channels_to_scan);
        puts("LEAVING MULTI READ MODE\n");
        } state multi_read
    }
    
    state single_read_response{
        when(delay(scan_speed)) {                       
            sscanf(single_channel_response, "%lf%s", &channels_readings[single_channel - 1], &channel_units[single_channel - 1]);
            pvPut(channels_readings[single_channel - 1]);
            pvPut(channel_units[single_channel - 1]);    
        } state prepare_to_read
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
        } state prepare_to_read
    }
}

