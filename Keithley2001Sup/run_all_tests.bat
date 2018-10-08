:: Run all tests
@echo off

SET TOP=".."

call %EPICS_KIT_ROOT%\config_env.bat

:: run tests
call %BIN%\runner.exe

set keithley_2001_tests_failed=%errorlevel%

:: Move XML file to test-reports folder

if not exist %TOP%\test-reports (
    mkdir %TOP%\test-reports
)

move "TEST-*.xml" "%TOP%\test-reports\"

exit /B %keithley_2001_tests_failed%
