@echo off
set log=%~n0.LOG & shift & set args=%*
if exist %log% del %log%
for %%a in (1 2 EXT1 EXT2 EXT3 EXT4) do call :run CPPU%%a.EXE
echo. & type %log%
echo. & echo Done.
exit/b

:run
 echo Running %1 inside DOSBox...
 echo *** %1 (%~z1 bytes) **********************************************>>%log%
 D:\bin\DOSBox\DOSBox.exe exit -c "%1 %args%>>%log%" -noconsole -exit
exit/b
