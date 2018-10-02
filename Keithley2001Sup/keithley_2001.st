program keithley_2001 ("P")

/*  This sequencer program runs the Keithley 2001.
 * 
 */ 
 
/* ==================== Sequencer compiler options =============================*/
 
option +r;
 
/* ==================== MACROS and includes =============================*/

#define MAX_INT_CHANNELS 8
%% #include "stdlib.h"
%% #include "stdio.h"
%% #include "string.h"
%% #include "dbDefs.h"
%% #include "seqPVmacros.h"

/* ==================== C function declarations =============================*/

%% void find_active_channels(int *arr, int number_of_active_channels, int *result);

/*=================== Variable declarations =========================*/

/* PVs triggered on startup */

int set_read_elements;
assign set_read_elements to "{P}ELEMENTS:SP.PROC";

int reset_device;
assign reset_device to "{P}RESET.PROC";

int reset_status_registers;
assign reset_status_registers to "{P}STAT:RESET.PROC";

int buffer_source = 1;
assign buffer_source to "{P}BUFF:SOURCE:SP.VAL";

int buffer_egroup = 0;
assign buffer_egroup to "{P}BUFF:EGROUP:SP.VAL";

int set_register_status_buffer_full;
assign set_register_status_buffer_full to "{P}STAT:BUFF:FULL:SP.PROC";

int set_measure_summary_register_bit;
assign set_measure_summary_register_bit to "{P}STAT:MEAS:SUMM:SP.PROC";

int set_scan_count = 1;
assign set_scan_count to "{P}SCAN:COUNT:SP.VAL";


/* Buffer control PVs */

int clear_buffer;
assign clear_buffer to "{P}BUFF:CLEAR.PROC";

int set_buffer_mode;
assign set_buffer_mode to "{P}BUFF:MODE:SP";

int buffer_size = MAX_INT_CHANNELS;
assign buffer_size to "{P}BUFF:SIZE:SP.VAL";

/* Channel PVs */

int active_channels[MAX_INT_CHANNELS];
assign active_channels to 
        {"{P}CHAN:01:ACTIVE",
        "{P}CHAN:02:ACTIVE",
        "{P}CHAN:03:ACTIVE",
        "{P}CHAN:04:ACTIVE",
        "{P}CHAN:06:ACTIVE",
        "{P}CHAN:07:ACTIVE",
        "{P}CHAN:08:ACTIVE",
        "{P}CHAN:09:ACTIVE"};
monitor active_channels;

double channels_readings[MAX_INT_CHANNELS];
assign channels_readings to 
        {"{P}CHAN:01:READ",
        "{P}CHAN:02:READ",
        "{P}CHAN:03:READ",
        "{P}CHAN:04:READ",
        "{P}CHAN:06:READ",
        "{P}CHAN:07:READ",
        "{P}CHAN:08:READ",
        "{P}CHAN:09:READ"};
monitor channels_readings;

string channel_units[MAX_INT_CHANNELS];
assign channel_units to 
        {"{P}CHAN:01:UNIT",
        "{P}CHAN:02:UNIT",
        "{P}CHAN:03:UNIT",
        "{P}CHAN:04:UNIT",
        "{P}CHAN:06:UNIT",
        "{P}CHAN:07:UNIT",
        "{P}CHAN:08:UNIT",
        "{P}CHAN:09:UNIT"};
monitor channel_units;


/* READING PVS */

int scan_mode;
assign scan_mode to "{P}READ:MODE.VAL";

int number_of_active_channels;
assign number_of_active_channels to "{P}READ:_NUM_OF_CHANS.VAL";
monitor number_of_active_channels;

string set_single_chan;
assign set_single_chan to "{P}READ:SINGLE:SP.VAL";

int read_single;
assign read_single to "{P}READ:SINGLE.PROC";

string single_channel_response;
assign single_channel_response to "{P}READ:SINGLE.VAL";
monitor single_channel_response;

double scan_speed;
assign scan_speed to "{P}READ:SCAN:SPEED.VAL";
monitor scan_speed;

/* Scan setup PVs */

long measurement_scan_count_setpoint;
assign measurement_scan_count_setpoint to "{P}SCAN:MEAS:COUNT:SP.VAL";


/* Non PV varaibles */

char *P;
string single_close_format;

/* Make sure that scanning_channels and previous_channel have the same type */
foreign scanning_channels;
%%int *scanning_channels;

int single_channel;
string read_channels;

int position;

int previous_channel;
int previous_scan_mode;
int previous_number_of_channels;

foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 



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
            puts("====ENTERING SINGLE_READ MODE====\n");
            scanning_channels = malloc(sizeof(previous_channel) * number_of_active_channels + 1);
            
           
            find_active_channels(active_channels, number_of_active_channels, scanning_channels);            
            single_channel = scanning_channels[0];
            free(scanning_channels);
            
            if (single_channel != previous_channel){
                printf("====CHANGING TO CHANNEL %i from %i ====\n", single_channel, previous_channel);
                
                sprintf(set_single_chan, "%d", single_channel);
                pvPut(set_single_chan);  
                previous_channel = single_channel;
                printf("====CHANGED TO CHANNEL %i ====\n", previous_channel);
            }
            printf("====READING CHANNEL %i\n", single_channel);
            
            pvPut(read_single);
            puts("====LEAVING SINGLE_READ MODE====\n");
        } state single_read_response
        
        when(number_of_active_channels > 1) {
            puts("====Going into MULTI READ MODE====\n");
            if (previous_scan_mode != 2) {
                scan_mode = 2;
                pvPut(scan_mode); 
                previous_scan_mode = scan_mode;
            }
            printf("====Scanning %i channels ====\n", number_of_active_channels);    
            
            if (previous_number_of_channels != number_of_active_channels){
                previous_number_of_channels = number_of_active_channels;
                
                buffer_size = previous_number_of_channels;
                pvPut(buffer_size);
                
                measurement_scan_count_setpoint = previous_number_of_channels;
                pvPut(measurement_scan_count_setpoint);
            }
            
            set_buffer_mode = 1; /* Buffer mode = NEXT */
            pvPut(set_buffer_mode);
            pvPut(clear_buffer);
      
            puts("====LEAVING MULIT READ MODE====\n");
        } state multi_read_response
    }
    
    state single_read_response{
        when(delay(scan_speed)) {
            puts("====Entering SINGLE READ RESPONSE====\n");
            
            position = (single_channel < 5) ? (single_channel - 1) : (single_channel - 2);
            
            sscanf(single_channel_response, "%lf%s", &channels_readings[position], &channel_units[position]);
            pvPut(channels_readings[position]);
            pvPut(channel_units[position]);           
                       
            puts("====Leaving SINGLE READ RESPONSE====\n");     
        } state read_channel
    }
    
    state multi_read_response {
        when(delay(scan_speed)){
            puts("====MULTI_READING====\n");
        } state read_channel
    }
}

/*==================== C helper functions =====================*/

%{

void find_active_channels(int *arr, int number_of_active_channels, int *result) {
    int position = 0;

    for (int i = 0; i < MAX_INT_CHANNELS; i++){
      if (arr[i] == 1 && position < MAX_INT_CHANNELS) {
        if (i < 4) {
          result[position] = i + 1;
        }
        else {
          result[position] = i + 2;
        }
        position++;
      }
    }
}

}%
