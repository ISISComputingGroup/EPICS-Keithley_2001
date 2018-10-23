:: Run all tests
@echo off

SET TOP="."

:: run tests
call Keithley2001Sup\src\O.windows-x64\runner.exe

set keithley_2001_tests_failed=%errorlevel%

:: Move XML file to test-reports folder

if not exist %TOP%\test-reports (
    mkdir %TOP%\test-reports
)

move "TEST-*.xml" "%TOP%\test-reports\"

exit /B %keithley_2001_tests_failed%
