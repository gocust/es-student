@echo off
mkdir build
cd build
echo o da vse systemi vklucheni
cmake -G "Unix Makefiles" ..
make
pause