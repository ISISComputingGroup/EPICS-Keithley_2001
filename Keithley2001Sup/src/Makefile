.PHONY: runner.cpp

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
Keithley_2001_SRCS += KeithleyUtils.cpp

# Test Runner

PROD_IOC += runner
USR_CPPFLAGS += -I$(CXXTEST)
runner_SRCS += runner.cpp KeithleyUtils.cpp
runner_LIBS += $(EPICS_BASE_IOC_LIBS)
runner_LIBS += Keithley_2001
runner_SYS_LIBS_WIN32 += wldap32

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

runner.cpp: 
	call $(CXXTEST)/bin/cxxtestgen --xunit-printer -w keithley2001 -o runner.cpp ../../tests/*.h
