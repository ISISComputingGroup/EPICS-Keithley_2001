# Makefile for Asyn keithley_2001 support
#
# Created by olz75487 on Fri Sep  7 16:44:38 2018
# Based on the Asyn streamSCPI template

TOP = .
include $(TOP)/configure/CONFIG

# Directories to build, any order
DIRS += configure
DIRS += $(wildcard *Sup)
DIRS += $(wildcard *App)
DIRS += $(wildcard *Top)
DIRS += $(wildcard iocBoot)

# The build order is controlled by these dependency rules:

# All dirs except configure depend on configure
$(foreach dir, $(filter-out configure, $(DIRS)), \
    $(eval $(dir)_DEPEND_DIRS += configure))

# Any *App dirs depend on all *Sup dirs
$(foreach dir, $(filter %App, $(DIRS)), \
    $(eval $(dir)_DEPEND_DIRS += $(filter %Sup, $(DIRS))))

# Any *Top dirs depend on all *Sup and *App dirs
$(foreach dir, $(filter %Top, $(DIRS)), \
    $(eval $(dir)_DEPEND_DIRS += $(filter %Sup %App, $(DIRS))))

# iocBoot depends on all *App dirs
iocBoot_DEPEND_DIRS += $(filter %App,$(DIRS))

# Add any additional dependency rules here:

include $(TOP)/configure/RULES_TOP

.PHONY: test
test:
ifeq ($(findstring 10.0,$(VCVERSION)),)
	$(TOP)/Keithley2001Sup/src/O.$(EPICS_HOST_ARCH)/runner --gtest_output=xml:$(TOP)/test-reports/TEST-Keithley2001.xml
endif

