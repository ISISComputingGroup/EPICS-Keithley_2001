TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------

# Keithley 2001 Libary

LIBRARY_IOC = Keithley_2001

Keithley_2001_LIBS += seqDev seq pv
Keithley_2001_LIBS += $(EPICS_BASE_IOC_LIBS)

Keithley_2001_SRCS += keithley_2001.st
Keithley_2001_SRCS += sequencer_utils.cpp sequencer_utils_cpp.cpp

# aSub record

Keithley_2001_LIBS += asyn utilities
Keithley_2001_LIBS += asubFunctions
Keithley_2001_SRCS += buffer_parsing_functions.c buffer_parsing.cpp buffer_parsing_utils.cpp

# Test Runner
TESTPROD_HOST += runner
runner_CPPFLAGS += -I$(CXXTEST)
runner_SRCS += runner.cpp sequencer_utils.cpp sequencer_utils_cpp.cpp buffer_parsing.cpp buffer_parsing_utils.cpp
runner_LIBS += Keithley_2001
runner_SYS_LIBS_WIN32 += wldap32

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

../runner.cpp:
	call $(CXXTEST)/bin/cxxtestgen --xunit-printer -w keithley2001 -o runner.cpp ../../tests/*tests.h
