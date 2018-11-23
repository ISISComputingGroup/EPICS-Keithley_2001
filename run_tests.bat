:: Run all tests
@echo off

SET TOP="."

set keithley_2001_tests_failed=%errorlevel%

:: run tests
call Keithley2001Sup\src\O.windows-x64\runner.exe --gtest_output=xml:%TOP%\test-reports\TEST-keithley_2001.xml

exit /B %keithley_2001_tests_failed%
