@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by TIMEDIM.HPJ. >"hlp\timedim.hm"
echo. >>"hlp\timedim.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\timedim.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\timedim.hm"
echo. >>"hlp\timedim.hm"
echo // Prompts (IDP_*) >>"hlp\timedim.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\timedim.hm"
echo. >>"hlp\timedim.hm"
echo // Resources (IDR_*) >>"hlp\timedim.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\timedim.hm"
echo. >>"hlp\timedim.hm"
echo // Dialogs (IDD_*) >>"hlp\timedim.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\timedim.hm"
echo. >>"hlp\timedim.hm"
echo // Frame Controls (IDW_*) >>"hlp\timedim.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\timedim.hm"
REM -- Make help for Project TIMEDIM


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\timedim.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\timedim.hlp" goto :Error
if not exist "hlp\timedim.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\timedim.hlp" Debug
if exist Debug\nul copy "hlp\timedim.cnt" Debug
if exist Release\nul copy "hlp\timedim.hlp" Release
if exist Release\nul copy "hlp\timedim.cnt" Release
echo.
goto :done

:Error
echo hlp\timedim.hpj(1) : error: Problem encountered creating help file

:done
echo.
