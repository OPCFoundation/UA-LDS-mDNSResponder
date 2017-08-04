@ECHO off
REM ****************************************************************************************************************
REM ** This script builds the mDNSresponder
REM ** This must be run from a Visual Studio command line.
REM ****************************************************************************************************************
SETLOCAL

set SRCDIR=%~dp0
set INSTALLDIR=%~dp0
set GIT=C:\Program Files (x86)\Git\bin\git.exe
set SIGNTOOL=C:\Build\sign_output.bat

IF "%1"=="no-clean" GOTO noClean
ECHO STEP 1) Deleting Output Directories
IF EXIST %INSTALLDIR%\bin rmdir /s /q %INSTALLDIR%\bin
:noClean

ECHO STEP 2) mDNSresponder
cd %SRCDIR%
msbuild mDNSResponder.sln /p:Configuration=Release /p:Platform=Win32
XCOPY /S /Q /Y %SRCDIR%\mDNSWindows\SystemService\Win32\Release\*.exe %INSTALLDIR%\bin\

ECHO STEP 5) Sign the Binaries
IF EXIST "%SIGNTOOL%" CALL "%SIGNTOOL%" %INSTALLDIR%\bin\*.exe /dual

ECHO *** ALL DONE ***
GOTO theEnd

:theEnd
ENDLOCAL