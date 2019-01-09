TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------

# Keithley 2001 Library

LIBRARY_IOC = Keithley_2001

Keithley_2001_LIBS += seqDev seq pv
Keithley_2001_LIBS += $(EPICS_BASE_IOC_LIBS)

Keithley_2001_SRCS += keithley_2001.st
Keithley_2001_SRCS += sequencer_utils.cpp 

# aSub record

Keithley_2001_LIBS += asyn utilities
Keithley_2001_LIBS += asubFunctions
Keithley_2001_SRCS += buffer_parsing_functions.c buffer_parsing.cpp buffer_parsing_utils.cpp

# googleTest Runner
ifneq ($(findstring windows,$(EPICS_HOST_ARCH)),)
ifeq ($(findstring 10.0,$(VCVERSION)),)

SRC_DIRS += $(TOP)/Keithley2001Sup/tests

TESTPROD_HOST += runner
USR_CPPFLAGS += -I$(GTEST)/googletest/include 
runner_SRCS += run_all_tests.cc
runner_SRCS +=  buffer_parsing_utils.cpp input_parsing_tests.cc output_setting_tests.cc
runner_SRCS +=  buffer_parsing_tests.cc
runner_SRCS += sequencer_utils_tests.cc sequencer_utils.cpp
runner_LIBS += gtest
endif
endif

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
