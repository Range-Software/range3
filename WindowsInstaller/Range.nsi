;NSIS Modern User Interface

;--------------------------------
;Include Modern UI

!include "MUI2.nsh"
!include "x64.nsh"

;--------------------------------
;Includes

!define ALL_USERS
;!include "AddToPath.nsh"
;!include "WriteEnvStr.nsh"

;--------------------------------
;General

;Name and file
Name "Range Software Package"
OutFile "C:\Work\range3\WindowsInstaller\range-3.0.alpha_3-installer.exe"

;Icon "C:\Work\range\WindowsInstaller\range.ico"
;UninstallIcon "C:\Work\Range-Software\range\WindowsInstaller\range.ico"

;Default installation folder
InstallDir "$PROGRAMFILES\Range Software 3"

;Get installation folder from registry if available
;InstallDirRegKey HKCU "Software\Range Software Package" ""

;Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------
;Variables

Var StartMenuFolder

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

!define MUI_ICON "C:\Work\range3\WindowsInstaller\range.ico"
!define MUI_UNICON "C:\Work\range3\WindowsInstaller\range.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "C:\Work\range3\WindowsInstaller\range-install.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "C:\Work\range3\WindowsInstaller\range-uninstall.bmp"
!define MUI_FINISHPAGE_LINK_LOCATION "http://www.range-software.com"
!define MUI_FINISHPAGE_LINK "http://www.range-software.com"
!define MUI_FINISHPAGE_RUN "$INSTDIR\bin\Range.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Run Range"
!define MUI_LICENSEPAGE_RADIOBUTTONS

;--------------------------------
;Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "C:\Work\build-range3\doc\LICENSE.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!define MUI_PAGE_CUSTOMFUNCTION_PRE ClosePrograms
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

; Print close programs warning
Function ClosePrograms
    MessageBox MB_OK "Please close all running programs before proceeding with installation."
FunctionEnd


Section "-global"
    SectionIn RO
    SetOutPath "$INSTDIR"

    CreateDirectory "$INSTDIR\bin"

    StrCpy $StartMenuFolder "Range Software 3"

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder\Documentation"
    CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
SectionEnd

SectionGroup "Range Software" SecRange
    Section "Executables" SecExeFiles
        SectionIn RO
        SetOutPath "$INSTDIR\bin"
        File C:\Work\build-range3\bin\Range.exe
        File C:\Work\build-range3\bin\RangeSolver.exe
        File C:\Work\range3\WindowsInstaller\bin\*.dll
        SetOutPath "$INSTDIR\bin\imageformats"
        File C:\Work\range3\WindowsInstaller\bin\imageformats\*
        SetOutPath "$INSTDIR\bin\platforms"
        File C:\Work\range3\WindowsInstaller\bin\platforms\*
        SetOutPath "$INSTDIR\doc"
        File C:\Work\build-range3\doc\LICENSE.txt
        File C:\Work\build-range3\doc\RELEASE_NOTES.txt
    SectionEnd
    Section "Documentation" SecDocFiles
        SetOutPath "$INSTDIR\doc"
        File C:\Work\build-range3\doc\*pdf
    SectionEnd
    Section "Model Files" SecModelFiles
        SetOutPath "$INSTDIR\data"
;        File C:\Work\build-range3\data\*.stl
        File C:\Work\build-range3\data\*.rbm
    SectionEnd
    Section "Material Files" SecMaterialFiles
        SetOutPath "$INSTDIR\materials"
        File C:\Work\build-range3\materials\*.rbmt
    SectionEnd
    SectionGroup "Shortcuts" SecShortcuts
        Section "Start Menu" SecShortcutsStart
            SectionIn RO
            SetOutPath "$INSTDIR"
            CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Range.lnk" "$INSTDIR\bin\Range.exe"
        SectionEnd
        Section "Desktop" SecShortcutsDesk
            SetOutPath "$INSTDIR"
            ;ADD YOUR OWN FILES HERE...
            CreateShortCut "$DESKTOP\Range.lnk" "$INSTDIR\bin\Range.exe"
        SectionEnd
    SectionGroupEnd
SectionGroupEnd

;--------------------------------
;Descriptions
    ;Language strings
    ;Range Software
    LangString DESC_SecRange          ${LANG_ENGLISH} "Range Software files."
    LangString DESC_SecDocFiles       ${LANG_ENGLISH} "Documentation."
    LangString DESC_SecModelFiles     ${LANG_ENGLISH} "Sample models and project files."
    LangString DESC_SecMaterialFiles  ${LANG_ENGLISH} "Material database files."
    LangString DESC_SecShortcuts      ${LANG_ENGLISH} "Shortcuts."
    LangString DESC_SecShortcutsStart ${LANG_ENGLISH} "Create a Start Menu entries."
    LangString DESC_SecShortcutsDesk  ${LANG_ENGLISH} "Create a shortcuts on the Desktop."

    ;Assign language strings to sections
    !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
        ;Range Software
        !insertmacro MUI_DESCRIPTION_TEXT ${SecRange}          $(DESC_SecRange)
        !insertmacro MUI_DESCRIPTION_TEXT ${SecDocFiles}       $(DESC_SecDocFiles)
        !insertmacro MUI_DESCRIPTION_TEXT ${SecModelFiles}     $(DESC_SecModelFiles)
        !insertmacro MUI_DESCRIPTION_TEXT ${SecMaterialFiles}  $(DESC_SecMaterialFiles)
        !insertmacro MUI_DESCRIPTION_TEXT ${SecShortcuts}      $(DESC_SecShortcuts)
        !insertmacro MUI_DESCRIPTION_TEXT ${SecShortcutsStart} $(DESC_SecShortcutsStart)
        !insertmacro MUI_DESCRIPTION_TEXT ${SecShortcutsDesk}  $(DESC_SecShortcutsDesk)
    !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"
    Delete "$INSTDIR\Uninstall.exe"

    ;Directories
    RMDir /r "$INSTDIR"
    RMDir /r "$SMPROGRAMS\$StartMenuFolder"

    Delete "$DESKTOP\Range.lnk"
SectionEnd