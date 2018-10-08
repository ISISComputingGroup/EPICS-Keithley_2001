.PHONY: runner.cpp

TOP=..
include $(TOP)/configure/CONFIG
#=======================================

LIBRARY_IOC = Keithley_2001
PROD_IOC += runner

# Install .dbd and .db files
DB += keithley_2001_misc.db
DB += keithley_2001_init.db
DB += keithley_2001_buffer.db
DB += keithley_2001_channels.db
DB += keithley_2001_channel_scanning.db

DATA += keithley_2001.proto

DBD += keithley_2001_sm.dbd

# Sequence file

Keithley_2001_LIBS += seqDev seq pv
Keithley_2001_LIBS += $(EPICS_BASE_IOC_LIBS)
Keithley_2001_SRCS += keithley_2001.st

# Helper functions written in C++
Keithley_2001_SRCS += KeithleyUtils.cpp

# Test Runner complie
USR_CPPFLAGS += -I$(CXXTEST)

runner_SRCS += runner.cpp KeithleyUtils.cpp
runner_LIBS += $(EPICS_BASE_IOC_LIBS)
runner_LIBS += Keithley_2001
runner_SYS_LIBS_WIN32 += wldap32


runner.cpp: 
	call $(CXXTEST)/bin/cxxtestgen --xunit-printer -w keithley2001 -o runner.cpp ../tests/*.h


#=======================================
include $(TOP)/configure/RULES
