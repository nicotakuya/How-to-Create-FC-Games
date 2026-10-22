echo off
echo demoから（main.c以外の)ソースファイルをコピーしてからビルドします。
pause
copy ..\demo\build.cfg
copy ..\demo\fcsub.c
copy ..\demo\fcsub.h
copy ..\demo\pattern.h
copy ..\demo\startup.asm
copy ..\demo\build.bat
copy ..\demo\clean.bat
build.bat
