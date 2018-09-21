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

int cler_buffer;
assign cler_buffer to "{P}BUFF:CLEAR.PROC";

char *P;
foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 

ss running {
    state init {
        when() {
            P = macValueGet("P");
            pvPut(reset_device);
            pvPut(set_read_elements);
            pvPut(cler_buffer);           
        } state reading
    } 
    state reading {
        when (delay(1)) {
            printf("====READING====\n");
        } state reading
    }
}
