@echo off
if "%1" == "" goto end
if exist %~n1.exe del %~n1.exe
g++ @gcc-opts -o %~n1.exe %~n1.cpp Lusbapi.lib
if exist %~n1.exe strip %~n1.exe
:end
