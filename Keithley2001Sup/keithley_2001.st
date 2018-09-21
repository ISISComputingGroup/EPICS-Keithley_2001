program keithley_2001 ("P, channels")

/*  This sequencer program works with Keithley 2001.
 * 
 */ 
 
#define MAX_INT_CHANNELS 10
%% #include "stdlib.h"
%% #include "string.h"
%% #include "dbDefs.h"
%% #include "seqPVmacros.h"
%% #include <windows.h>

int read_elements_sp;
assign read_elements_sp to "{P}ELEMENTS:SP.PROC";

char *P;
foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 

ss running {
    state init {
        when() {
            /* Initialize the IOC for scanning correctly */
            P = macValueGet("P");
            
            /* Initialize elements of a reading */
            pvPut(read_elements_sp);
        } state reading
    } 
    state reading {
        when (delay(1)) {
            printf("====READING====\n");
        } state reading
    }
}
