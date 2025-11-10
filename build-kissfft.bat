@echo off
set W64DEVKIT_X64_BIN_PATH=C:\progs\w64devkit-x64\bin
set PATH=%W64DEVKIT_X64_BIN_PATH%;%PATH%

make -C kissfft-131.2.0 KISSFFT_DATATYPE=float KISSFFT_TOOLS=0 KISSFFT_TEST=0 KISSFFT_STATIC=1 all
gcc -I ./kissfft-131.2.0 main2.c -o main2.exe -L ./kissfft-131.2.0 -lkissfft-float