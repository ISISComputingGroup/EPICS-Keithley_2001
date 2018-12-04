TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this

DB += keithley_2001_misc.db
DB += keithley_2001_init.db
DB += keithley_2001_buffer.db
DB += keithley_2001_channel.db
DB += keithley_2001_channel_scanning.db
DB += keithley_2001_buffer_parsing.db
DB += keithley_2001_error_handling.db
DB += unit_setter.db
DB += alarm_readings_setter.db
DB += alarm_units_setter.db


#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

