@echo off
set log=%~n0.LOG
if exist %log% del %log%
for %%a in (1 2 EXT1 EXT2 EXT3 EXT4) do call :run CPPU%%a.EXE %1
echo Done.
exit/b

:run
 echo Running %1...
 echo *** %1 (%~z1 bytes) **********************************************>>%log%
 D:\bin\DOSBox\DOSBox.exe exit -c "%1 %2>>%log%" -noconsole -exit
exit/b
