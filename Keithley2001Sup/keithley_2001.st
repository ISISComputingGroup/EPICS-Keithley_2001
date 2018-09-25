program keithley_2001 ("P, channels")

/*  This sequencer program works with Keithley 2001.
 * 
 */ 
 
#define MAX_INT_CHANNELS 8
%% #include "stdlib.h"
%% #include "string.h"
%% #include "dbDefs.h"
%% #include "seqPVmacros.h"
%% int array_sum(int array[], int length_of_array);

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

char *P;
foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 

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

int num_of_active_channels;

int scan_mode;
assign scan_mode to "{P}READ:MODE";

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
        } state before_reading
    }
    state before_reading {
        when(delay(1)) {
            num_of_active_channels = array_sum(active_channels, MAX_INT_CHANNELS);
        } state select_read_mode
    }
    state select_read_mode {
        when(num_of_active_channels == 0) {
            scan_mode = 0;
            pvPut(scan_mode);
            puts("====Going into IDLE MODE====");
        } state before_reading
        when(num_of_active_channels == 1) {
            scan_mode = 1;
            pvPut(scan_mode);
            puts("====Going into SINGLE READ MODE====");
        } state single_read        
        when(num_of_active_channels > 1) {
            scan_mode = 2;
            pvPut(scan_mode);
            puts("====Going into MULTI READ MODE====");
        } state multi_read
    }
    
    state single_read{
        when(delay(1)){
            puts("====SINGLE_READING====");
        } state before_reading
    } 
    state multi_read {
        when(delay(1)){
            puts("====MULTI_READING====");
        } state before_reading
    }
}

%% int array_sum(int array[], int length_of_array){
%%    int total = 0;
%%
%%    for (int i = 0; i < length_of_array; i++) {
%%        total += array[i];
%%    }
%%    
%%    return total;
%%}
