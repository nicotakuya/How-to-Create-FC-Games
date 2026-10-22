echo off
echo demoと重複するファイルを削除(main.cとrom.nesだけ残す)。
pause
del build.cfg
del fcsub.c
del fcsub.h
del pattern.h
del startup.asm
del build.bat
del clean.bat
del *.o
del *.nes.deb
