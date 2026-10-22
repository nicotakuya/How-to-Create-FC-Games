echo off
echo ROMイメージのビルドを行います
set SRC1=startup
set SRC2=main
set SRC3=fcsub
set CFGFILE=build.cfg
set COMPILE=C:\cc65\bin\cl65
set LINK=C:\cc65\bin\ld65
set LIBFILE=C:\cc65\lib\nes.lib
set NESFILE=rom.nes
%COMPILE% -t none -o %SRC1%.o -c %SRC1%.asm
if %errorlevel% neq 0 goto abort
%COMPILE% -t none -o %SRC2%.o -c -O %SRC2%.c
if %errorlevel% neq 0 goto abort
%COMPILE% -t none -o %SRC3%.o -c -O %SRC3%.c
if %errorlevel% neq 0 goto abort
%LINK% -o %NESFILE% --config %CFGFILE% ^
 --obj %SRC1%.o %SRC2%.o %SRC3%.o --lib %LIBFILE%
if %errorlevel% neq 0 goto abort
echo ビルドに成功しました
:abort
pause
