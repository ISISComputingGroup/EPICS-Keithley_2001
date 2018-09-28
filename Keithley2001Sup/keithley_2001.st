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
assign buffer_source to "{P}BUFF:SOURCE:SP";

int buffer_mode = 2;
assign buffer_mode to "{P}BUFF:MODE:SP";

int buffer_size = 250;
assign buffer_size to "{P}BUFF:SIZE:SP";

int buffer_egroup = 0;
assign buffer_egroup to "{P}BUFF:EGROUP:SP";

int scanning_continuously_sp = 1;
assign scanning_continuously_sp to "{P}SCAN:CONT_MODE:SP";

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

int scan_mode;
assign scan_mode to "{P}READ:MODE";

string read_channels;
assign read_channels to "{P}READ:CHANNELS.VAL";

int number_of_active_channels;
assign number_of_active_channels to "{P}READ:_NUM_OF_CHANS.VAL";
monitor number_of_active_channels;

char *P;
foreign SIZE_OF_INT;
%%int SIZE_OF_INT = sizeof(int);

foreign scanning_channels;
%%int *scanning_channels;
foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 


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
        } state read_mode
    }
    state read_mode {
        
        when(number_of_active_channels == 0) {
            scan_mode = 0;
            pvPut(scan_mode);
        } state read_mode
        
        when(number_of_active_channels == 1) {
            scan_mode = 1;
            pvPut(scan_mode);            
            puts("====ENTERING SINGLE_READ MODE====");
            scanning_channels = malloc(SIZE_OF_INT * number_of_active_channels + 1);
            
            find_active_channels(active_channels, number_of_active_channels, scanning_channels);
            generate_active_channel_string(scanning_channels, number_of_active_channels, read_channels);
            
            printf("READING CHANNEL %s", read_channels);
            pvPut(read_channels);
            
            free(scanning_channels);
            puts("====LEAVING SINGLE_READ MODE====");
        } state read_mode
        
        when(number_of_active_channels > 1) {
            scan_mode = 2;
            pvPut(scan_mode);
            puts("====Going into MULTI READ MODE====");
        } state multi_read
    }
    

    state multi_read {
        when(delay(1)){
            puts("====MULTI_READING====");
        } state read_mode
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

void generate_active_channel_string(int *arr, int num_of_active_channels, char *result) {
  sprintf(result, "%d", arr[0]);
}

}%

