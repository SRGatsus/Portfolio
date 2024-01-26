@echo off
set EXEFILE=e140-wav-player.exe
if exist "%EXEFILE%" del "%EXEFILE%"
g++ -static-libstdc++ -static-libgcc -O2 @gcc-warnings -o "%EXEFILE%" -I.. main.cpp *.c ..\Lusbapi.lib
if exist "%EXEFILE%" strip "%EXEFILE%"
