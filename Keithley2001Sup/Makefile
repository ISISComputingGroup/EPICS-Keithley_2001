TOP=..
include $(TOP)/configure/CONFIG
#=======================================

# Install .dbd and .db files
DB += keithley_2001.db
DB += keithley_2001_init.db
DB += keithley_2001_channels.db
DB += keithley_2001_channel_scanning.db
DATA += keithley_2001.proto

DBD += keithley_2001_sm.dbd

# Sequence file
LIBRARY_IOC = Keithley_2001
Keithley_2001_LIBS += seqDev seq pv
Keithley_2001_LIBS += $(EPICS_BASE_IOC_LIBS)

Keithley_2001_SRCS += keithley_2001.st

#=======================================
include $(TOP)/configure/RULES
