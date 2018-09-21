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

int scanning;
assign scanning to "{P}SCANNING";
monitor scanning;

char *P;
foreign pvname;
%%char pvname[PVNAME_STRINGSZ]; 

ss running {
    state init {
        when() {
            /* Initialize the IOC for scanning correctly */
            P = macValueGet("P");
        } state reading
    } 
    state reading {
        when (delay(0.5)) {
            printf("===P is %s", P);
            printf("====READING====\n");
            printf("===============\n");
        } state reading
    }
}
