program keithley_2001 ("P, channels")

/*  This sequencer program works with Keithley 2001.
 * 
 */ 
 
#define MAX_INT_CHANNELS 10
%% #include "stdlib.h"
%% #include "string.h"
%% #include "dbDefs.h"
%% #include "seqPVmacros.h"

int set_read_elements;
assign set_read_elements to "{P}ELEMENTS:SP.PROC";

int reset_device;
assign reset_device to "{P}RESET.PROC";

int clear_buffer;
assign clear_buffer to "{P}BUFF:CLEAR.PROC";

int buffer_source_value = 1;
assign buffer_source_value to "{P}BUFF:SOURCE:SP";

char *P;
foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 

ss running {
    state init {
        when() {
            P = macValueGet("P");
            pvPut(reset_device);
            pvPut(set_read_elements);
            pvPut(clear_buffer);     
            pvPut(buffer_source_value);
        } state reading
    } 
    state reading {
        when (delay(1)) {
            printf("====READING====\n");
        } state reading
    }
}
