:: Run all tests
@echo off

SET CurrentDir=%~dp0

SET CXXTEST="%EPICS_KIT_ROOT%\libraries\master\cxxtest"

call %EPICS_KIT_ROOT%\config_env.bat

:: Add python dependency
call python -m pip install ply

cd %CXXTEST%\bin
:: Generate tests
call cxxtestgen --xunit-printer -o %CurrentDir%runner.cpp %CurrentDir%MyTestSuite1.h

cd %CurrentDir%
:: compile tests
call g++ -o %CurrentDir%runner -I %CXXTEST% %CurrentDir%runner.cpp

:: run tests
call runner.exe

set keithley_2001_tests_failed=%errorlevel%

:: Remove extra files
del runner.exe runner.cpp

:: Move XML file to test-reports folder

if not exist %CurrentDir%..\..\test-reports (
    mkdir %CurrentDir%..\..\test-reports
)

move "TEST-*.xml" "..\..\test-reports\"

exit /B %keithley_2001_tests_failed%
