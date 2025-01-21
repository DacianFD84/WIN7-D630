@echo off

    REM *************************************************
    REM *            SPATCH.BAT                         *
    REM *   Patch to allow swapfile.exe to work with    *
    REM *   msdos 6. Designed to be used only once      *
    REM *   for this specific purpose                   *
    REM *************************************************

set OLDPATH=%PATH%
if "%1" == "" goto usage
if "%1" == "/?" goto usage
if "%1" == "-?" goto usage

set WINPATH=%1
set PATH=%WINPATH%;%OLDPATH%
if not exist %WINPATH%\system\swapfile.exe goto ERROR2
if not exist %WINPATH%\winver.exe goto ERROR4
attrib -r -h -s %WINPATH%\system\swapfile.exe

echo                             NOTE
echo;
echo This batch program modifies your Windows 3.0/3.0a SWAPFILE.EXE file for
echo use with MS-DOS 6. A backup of the file will be saved as SWAPFILE.SAV
echo;
echo;

pause

find "3.00a" %WINPATH%\WINVER.EXE > nul

if not errorlevel 1 goto win3a

find "3.00" %WINPATH%\WINVER.EXE > nul

if not errorlevel 1 goto win30

goto ERROR5


:WIN30
set ADDR=2dc0
goto modswap

:WIN3A
set ADDR=2df2
              
:MODSWAP
copy   %WINPATH%\system\swapfile.exe  swapfile.sav
copy   %WINPATH%\system\swapfile.exe  swapfile.dbg

echo n swapfile.dbg      > spatch.src
echo l                  >> spatch.src
echo e %ADDR% b8,5,0,90 >> spatch.src
echo w                  >> spatch.src
echo q                  >> spatch.src

debug swapfile.dbg       < spatch.src > NUL
if errorlevel goto error3

copy  swapfile.dbg  %WINPATH%\system\swapfile.exe
del   swapfile.dbg
goto success

:USAGE

    echo;
    echo    Patches the Windows 3.0/3.0a SWAPFILE.EXE for use with MS-DOS 6 
    echo.
    echo      SPATCH [drive:][Windows directory]
    echo.
    echo      EXAMPLE: SPATCH C:\WINDOWS
    echo;
    echo    NOTE: Make sure you use the correct path for Windows
    echo;

goto end

:ERROR1

   echo;
   echo    Can not find the Windows directory. Make sure you enter
   echo    the correct path.
   echo;

goto end

:ERROR2

   echo;
   echo    Can not find SWAPFILE.EXE in your WINDOWS\SYSTEM subdirectory.
   echo;

goto end


:ERROR3

   echo    Error patching swapfile.exe. Original swapfile.exe being restored.
   echo    Look at the readme file to answer any questions.
   echo;

copy swapfile.sav %WINPATH%\system\swapfile.exe
del  swapfile.sav

goto end

:ERROR4

   echo    Error.  Could not find WINVER.EXE.
   echo    Cannot determine your Windows version.

goto end

:ERROR5

   echo    Error.  Unrecognized Windows version.

goto end

:SUCCESS
   echo;
   echo    Your SWAPFILE.EXE file was modified successfully. After you confirm
   echo    that the file works correctly, you can delete SWAPFILE.SAV.
   echo;

if exist spatch.src del spatch.src

:END

set ADDR=
set WINPATH=
set PATH=%OLDPATH%
set OLDPATH=
