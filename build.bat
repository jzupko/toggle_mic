@echo off
setlocal

REM Check for cl.exe in the PATH, if not present, let the user
REM know to run vcvars64.bat
where cl.exe >nul 2>nul
if '%errorlevel%' neq '0' goto missing

REM Setup some convenience variables.
set BIN_DIR=%~dp0bin\Windows\Release\x64
set OBJ_DIR=%~dp0obj\Windows\Release\x64
set DEFINES=%* /D"_HAS_EXCEPTIONS=0" /D"_MBCS" /D"_WIN64" /D"WIN64" /D"NDEBUG"

REM Cleanup any existing build state.
rmdir /s /q %~dp0bin >nul 2>nul
rmdir /s /q %~dp0obj >nul 2>nul
mkdir %BIN_DIR%
mkdir %OBJ_DIR%

REM Compile code.
cl.exe "%~dp0src\toggle_mic.cpp" /Fo"%OBJ_DIR%\toggle_mic.obj" %DEFINES% /c /Z7 /nologo /W3 /WX /diagnostics:classic /Gm- /GS /fp:precise /fp:except- /Zc:wchar_t /Zc:forScope /Zc:inline /GR- /openmp- /Gd /FC /errorReport:prompt /Ox /GF /MT /Gy /Ob2 /Oi /Ot /GT
if '%errorlevel%' neq '0' goto fail

REM Compile resource.
rc.exe /nologo /fo"%OBJ_DIR%\toggle_mic.res" %DEFINES% "%~dp0src\toggle_mic.rc"
if '%errorlevel%' neq '0' goto fail

REM Link toggle_mic.exe.
link.exe /OUT:"%BIN_DIR%\toggle_mic.exe" "%OBJ_DIR%\toggle_mic.obj" "%OBJ_DIR%\toggle_mic.res" ole32.lib /ERRORREPORT:PROMPT /NOLOGO /MANIFEST:NO /DEBUG /TLBID:1 /NXCOMPAT /MACHINE:X64 /WX /Brepro /OPT:ICF /DYNAMICBASE /INCREMENTAL:NO /OPT:REF
if '%errorlevel%' neq '0' goto fail

:success
echo [SUCCESS]
exit /b 0

:fail
echo [FAIL]
exit /b 1

:missing
echo [FAIL]
echo Need a copy of Visual Studio in the environment. Run "x64 Native Tools Command Prompt" or "vcvars64.bat" in the shell prior to running build.bat.
exit /b 1
