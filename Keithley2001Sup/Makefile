TOP=..
include $(TOP)/configure/CONFIG
#=======================================

# Install .dbd and .db files
DB += keithley_2001.db
DB += keithley_2001_channels.db
DB += keithley_2001_view.db

DATA += Keithley_2001.proto

DBD += Keithley_2001.dbd
LIBRARY_IOC += Keithley_2001

Keithley_2001_DBD += asyn.dbd
Keithley_2001_DBD += utilities.dbd
Keithley_2001_DBD += asubFunctions.dbd
Keithley_2001_DBD += asSupport.dbd
Keithley_2001_DBD += stream.dbd
Keithley_2001_DBD += sseqRecord.dbd

Keithley_2001_LIBS += $(EPICS_BASE_IOC_LIBS)
Keithley_2001_LIBS += asyn utilities
Keithley_2001_LIBS += asubFunctions
#Keithley_2001_LIBS += sseqFunctions

Keithley_2001_SRCS += parse_buffer_readings.c

#=======================================
include $(TOP)/configure/RULES
