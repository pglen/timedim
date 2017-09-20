;///////////////////////////////////////////////////////////////////////////
;//

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;Name and file

  Name "TimeDim"

;--------------------------------
;Build target selection

    !echo "------------------------"
    !echo "Build TimeDim Original Version"

    OutFile ".\bin\setup.exe"
    !define BANNER  "logo.jpg"

    ;Default installation folder
    InstallDir "$PROGRAMFILES\TimeDim"

    BrandingText " TimeDim Copy Machine "

    ;Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\TimeDim" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS

  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

    !insertmacro MUI_LANGUAGE "English"

    icon TimeDim.ico
    uninstallicon TimeDim.ico

; -------------------------------------------------------------------------
;Installer Sections

Section "Install TimeDim Code" SecCode

    #!error "exit"

    SetShellVarContext all

    SetOutPath "$INSTDIR"

    ; Add your own files here...
    File  /oname=TimeDim.exe ..\Release\TimeDim.exe

    ; Copy logo to target
    ;File  /oname=logo.jpg ${BANNER}

    ; Copy OEM FILE to target
    ;File /oname=${XBANNER} oem.txt

    File  Readme.txt
    File  License.txt
    ;File  default.gesture

    ;File  Quick_Start.doc

    File  /r /x ".svn" /x "*.cpt" /x "*.xcpt" /x "_autosave" "html"

    SetOutPath "$SYSDIR"

    ;SetOverwrite off
    ;File  /nonfatal .\dll\MFC42D.DLL
    ;File  /nonfatal .\dll\MSVCRTD.DLL
    ;File  /nonfatal .\dll\MSVCN42D.DLL
    ;SetOverwrite on

    ; ---------------------------------------------------------------------
    SetOutPath $INSTDIR

    CreateDirectory $SMPROGRAMS\TimeDim
    CreateShortCut "$SMPROGRAMS\TimeDim\TimeDim.lnk" "$INSTDIR\TimeDim.exe" "" \
                    "$INSTDIR\TimeDim.exe" 0  SW_SHOWNORMAL  "ALT|CONTROL|T"

    CreateShortCut "$QUICKLAUNCH\TimeDim.lnk" "$INSTDIR\TimeDim.exe"
    CreateShortCut "$SENDTO\TimeDim.lnk" "$INSTDIR\TimeDim.exe"

;    CreateShortCut "$SMPROGRAMS\TimeDim\TimeDim Web Site.lnk" "http:\\www.TimeDim.com" "" \
;                    "" 0  SW_SHOWNORMAL  "ALT|CONTROL|W"

    CreateDirectory $SMPROGRAMS\TimeDim\Documents
    CreateShortCut "$SMPROGRAMS\TimeDim\Documents\Readme.lnk" "$INSTDIR\Readme.txt"
    CreateShortCut "$SMPROGRAMS\TimeDim\Documents\HTML Documentation.lnk" "$INSTDIR\html\toc.html"

;    CreateShortCut "$SMPROGRAMS\TimeDim\Documents\Quick Start.lnk" "$INSTDIR\Quick_Start.doc"

    CreateDirectory $SMPROGRAMS\TimeDim\Tools
    CreateShortCut "$SMPROGRAMS\TimeDim\Tools\Uninstall.lnk" "$INSTDIR\Uninstall.exe" \
                    "$INSTDIR\TimeDim.exe"

;    CreateShortCut "$SMPROGRAMS\TimeDim\Tools\License.lnk" "$INSTDIR\License.txt"
;    CreateShortCut "$DESKTOP\TimeDim.lnk" "$INSTDIR\TimeDim.exe"

    ;Store installation folder
    WriteRegStr HKCU "Software\TimeDim" "" $INSTDIR

    ; ---------------------------------------------------------------------
    ;Register TimeDim file as .TimeDim handler

    ;WriteRegStr HKCR ".TimeDim" "" "TimeDim.FileHandler"
    ;WriteRegStr HKCR "TimeDim.FileHandler" "" ""
    ;WriteRegStr HKCR "TimeDim.FileHandler\shell" "" "open"
    ;WriteRegStr HKCR "TimeDim.FileHandler\DefaultIcon" "" "$INSTDIR\TimeDim.exe,0"
    ;WriteRegStr HKCR "TimeDim.FileHandler\shell\open\command" "" '$INSTDIR\TimeDim.exe "%1"'
    ;WriteRegStr HKCR "TimeDim.FileHandler\shell\edit" "" ""
    ;WriteRegStr HKCR "TimeDim.FileHandler\shell\edit\command" "" '$INSTDIR\TimeDim.exe "%1"'

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; Create control panel unistaller
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"  "DisplayName" "TimeDim Copy Machine  "
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"  "UninstallString" $INSTDIR\Uninstall.exe
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"  "DisplayIcon" $INSTDIR\TimeDim.exe,0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"  "DisplayVersion" "Version 1.10"
    ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"  "HelpLink" "http://www.TimeDim.com"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"  "NoModify" 1
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"  "NoRepair" 1

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

    SetShellVarContext all

    ;ADD YOUR OWN FILES HERE...

    Delete "$INSTDIR\Uninstall.exe"
    Delete "$INSTDIR\TimeDim.exe"

    RMDir /r "$INSTDIR"

    Delete "$SMPROGRAMS\TimeDim\TimeDim.lnk"
    Delete "$SMPROGRAMS\TimeDim\Readme.lnk"
    Delete "$SMPROGRAMS\TimeDim\License.lnk"

    Delete "$SMPROGRAMS\TimeDim\Guide_inside.pdf"
    Delete "$SMPROGRAMS\TimeDim\Guide_outside.pdf"
    Delete "$SMPROGRAMS\TimeDim\manual.pdf"

    Delete "$QUICKLAUNCH\TimeDim.lnk"
    Delete "$DESKTOP\TimeDim.lnk"
    Delete "$SENDTO\TimeDim.lnk"

    RMDir /r "$SMPROGRAMS\TimeDim\Tools\"
    RMDir /r "$SMPROGRAMS\TimeDim\"

    DeleteRegKey HKCU "Software\TimeDim"
    DeleteRegKey HKCR ".TimeDim"
    DeleteRegKey HKCR "TimeDim.FileHandler"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TimeDim"

SectionEnd

  LangString DESC_SecCode ${LANG_ENGLISH} "Install TimeDim code.$\r$\n$\r$\n\
  UnCheck (unselect) this if you do not want the installation to modify or update the TimeDim executable."
;
;  LangString DESC_SecData ${LANG_ENGLISH} "Install TimeDim data.$\r$\n$\r$\n\
;  UnCheck (unselect) this if you do not want the installation to modify or update any of your implant systems."
;
;  LangString DESC_SecImp ${LANG_ENGLISH} "Install TimeDim Implants data.$\r$\n$\r$\n\
; UnCheck (unselect) this if you do not want the installation to modify or update any of your implant systems."
;
;  LangString DESC_SecGal ${LANG_ENGLISH} "Install TimeDim Galleries data.$\r$\n$\r$\n\
; UnCheck (unselect) this if you do not want the installation to modify or update any of your gallery items."
;
;  LangString DESC_SecPat ${LANG_ENGLISH} "Install TimeDim Sample patient data.$\r$\n$\r$\n\
; UnCheck (unselect) this if you do not want the installation to modify or update any of your patient items. (safe to leave it checked)"
;
;  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCode} $(DESC_SecCode)
;    !insertmacro MUI_DESCRIPTION_TEXT ${SecData} $(DESC_SecData)
;    !insertmacro MUI_DESCRIPTION_TEXT ${SecGal} $(DESC_SecGal)
;    !insertmacro MUI_DESCRIPTION_TEXT ${SecPat} $(DESC_SecPat)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
;

