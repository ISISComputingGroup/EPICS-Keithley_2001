:: Run all tests
@echo off

SET TOP="."

set Tests_failed=%errorlevel%

:: run tests
call Keithley2001Sup\src\O.windows-x64\runner.exe --gtest_output=xml:%TOP%\test-reports\TEST-Keithley2001.xml

exit /B %Tests_failed%
