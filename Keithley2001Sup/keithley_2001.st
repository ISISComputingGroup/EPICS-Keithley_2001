program keithley_2001 ("P, channels")

/*  This sequencer program works with Keithley 2001.
 * 
 */ 
 
/* ==================== Options =============================*/
 
option +r;
 
/* ==================== MACROS and includes =============================*/

#define MAX_INT_CHANNELS 8
%% #include "stdlib.h"
%% #include "stdio.h"
%% #include "string.h"
%% #include "dbDefs.h"
%% #include "seqPVmacros.h"

/* ==================== C function declarations =============================*/

%% void find_active_channels(int *arr, int num_of_active_channels, int *result);
%% void generate_active_channel_string(int *arr, int num_of_active_channels, char *result);



/*=================== Variable declarations =========================*/
int set_read_elements;
assign set_read_elements to "{P}ELEMENTS:SP.PROC";

int reset_device;
assign reset_device to "{P}RESET.PROC";

int clear_buffer;
assign clear_buffer to "{P}BUFF:CLEAR.PROC";

int buffer_source = 1;
assign buffer_source to "{P}BUFF:SOURCE:SP.VAL";

int buffer_mode = 2;
assign buffer_mode to "{P}BUFF:MODE:SP.VAL";

int buffer_size = 250;
assign buffer_size to "{P}BUFF:SIZE:SP.VAL";

int buffer_egroup = 0;
assign buffer_egroup to "{P}BUFF:EGROUP:SP.VAL";

int scanning_continuously_sp = 1;
assign scanning_continuously_sp to "{P}SCAN:CONT_MODE:SP.VAL";

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

int scan_mode;
assign scan_mode to "{P}READ:MODE.RVAL";

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


/* Non PV varaibles */

char *P;
string single_close_format;

/* Make sure that scanning_channels and previous_channel have the same type */
foreign scanning_channels;
%%int *scanning_channels;
int previous_channel;
int single_channel;
string read_channels;

int position;

int previous_scan_mode;

foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 

int number_of_reads = 0;


/*=================== State Sets ===========================*/

ss running {
    state init {
        when() {
            P = macValueGet("P");
            pvPut(reset_device);
            pvPut(set_read_elements);
            pvPut(clear_buffer);     
            pvPut(buffer_source);
            pvPut(buffer_mode);
            pvPut(buffer_size);
            pvPut(buffer_egroup);
            pvPut(scanning_continuously_sp);
            pvGet(scan_speed);
            previous_channel = 0;
            previous_scan_mode = -1;
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
            scan_mode = 2;
            pvPut(scan_mode);
            puts("====Going into MULTI READ MODE====\n");
        } state multi_read
    }
    
    state single_read_response{
        when(delay(scan_speed)) {
            puts("====Entering SINGLE READ RESPONSE====\n");
            
            number_of_reads += 1;
            position = (single_channel < 5) ? (single_channel - 1) : (single_channel - 2);
            
            sscanf(single_channel_response, "%lf%s", &channels_readings[position], &channel_units[position]);
            pvPut(channels_readings[position]);
            pvPut(channel_units[position]);           
                       
            puts("====Leaving SINGLE READ RESPONSE====\n");     
        } state read_channel
    }
    
    state multi_read {
        when(delay(scan_speed)){
            puts("====MULTI_READING====\n");
        } state read_channel
    }
}

/*==================== C helper functions =====================*/

%{

void find_active_channels(int *arr, int num_of_active_channels, int *result) {
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

