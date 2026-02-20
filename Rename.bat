@echo off
:: 自动切 UTF-8，防止中文乱码
for /f "tokens=2" %%i in ('chcp') do set "OLD_CP=%%i"
chcp 65001 >nul

setlocal enabledelayedexpansion
set "sourceFile=.project"
set "target_line="
for /f "tokens=1* delims=:" %%a in ('findstr /n "^" "%sourceFile%"') do (
    if %%a equ 3 set "target_line=%%b"
)
for /f "tokens=3 delims=<>" %%c in ("%target_line%") do set "old_str=%%c"
echo 当前工程名称: %old_str%
set /p "userInput=请输入新的工程名称: "
set "tempFile=temp.txt"
set "oldString=%old_str%"
set "newString=%userInput%"
if exist "%tempFile%" del /Q "%tempFile%"
for /f "delims=" %%i in ('type "%sourceFile%"') do (
    set "line=%%i"
    set "modified=!line!"
    if defined modified (
        set "modified=!modified:%oldString%=%newString%!"
        echo !modified!>>"%tempFile%"
    ) else (
        echo %%i>>"%tempFile%"
    )
)
move /y "%tempFile%" "%sourceFile%" >nul 2>&1

set "sourceFile=.cproject"
if exist "%tempFile%" del /Q "%tempFile%"
for /f "delims=" %%i in ('type "%sourceFile%"') do (
    set "line=%%i"
    set "modified=!line!"
    if defined modified (
        set "modified=!modified:%oldString%=%newString%!"
        echo !modified!>>"%tempFile%"
    ) else (
        echo %%i>>"%tempFile%"
    )
)
move /y "%tempFile%" "%sourceFile%" >nul 2>&1

:: 如有需要，把下面一行换成你自己的清理脚本
if exist "删除临时文件.bat" call "删除临时文件.bat"

:: 还原代码页
chcp %OLD_CP% >nul
endlocal