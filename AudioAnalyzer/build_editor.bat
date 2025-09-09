
REM @echo off

call "%~dp0\vars.bat"
call "%BUILD_BAT%" UnrealEditor Win64 Development "%UPROJECT_PATH%" -waitmutex -NoHotReload
