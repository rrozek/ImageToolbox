;NSIS Modern User Interface
;Welcome/Finish Page Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "PBS Image Toolbox"
  OutFile "PBSImageToolboxInstaller.exe"

  ;Default installation folder
  InstallDir "$LOCALAPPDATA\PBSImageToolbox"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\PBSImageToolbox" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
;  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
;  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  ;!insertmacro MUI_UNPAGE_WELCOME
  ;!insertmacro MUI_UNPAGE_CONFIRM
  ;!insertmacro MUI_UNPAGE_INSTFILES
  ;!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "ImageToolbox" SecToolbox

  SetOutPath "$INSTDIR"

  File /r deployment_stuff\*.*

  ;Store installation folder
  WriteRegStr HKCU "Software\PBSImageToolbox" "" $INSTDIR

     ; include for some of the windows messages defines
   !include "winmessages.nsh"
   ; HKLM (all users) vs HKCU (current user) defines
   !define env_hklm 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
   !define env_hkcu 'HKCU "Environment"'
   ; set variable for local machine
   ; and current user
   WriteRegExpandStr ${env_hklm} MAGICK_CONFIGURE_PATH $INSTDIR\im
   WriteRegExpandStr ${env_hkcu} MAGICK_CONFIGURE_PATH $INSTDIR\im
   ; make sure windows knows about the change
   SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
   
   WriteRegExpandStr ${env_hklm} MAGICK_CODER_MODULE_PATH $INSTDIR\im\modules\coders
   WriteRegExpandStr ${env_hkcu} MAGICK_CODER_MODULE_PATH $INSTDIR\im\modules\coders
   ; make sure windows knows about the change
   SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
   
   WriteRegExpandStr ${env_hklm} MAGICK_FILTER_MODULE_PATH $INSTDIR\im\modules\filters
   WriteRegExpandStr ${env_hkcu} MAGICK_FILTER_MODULE_PATH $INSTDIR\im\modules\filters
   ; make sure windows knows about the change
   SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
   
  ;Create uninstaller
  ;WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecToolbox ${LANG_ENGLISH} "Toolbox"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecToolbox} $(DESC_SecToolbox)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

;Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

;  Delete "$INSTDIR\Uninstall.exe"

;  RMDir "$INSTDIR"

  ;DeleteRegKey /ifempty HKCU "Software\PBSImageToolbox"

;SectionEnd

;!finalize 'sign.bat "%1" "PBSImageToolboxInstaller" https://www.pbsnetwork.eu/'