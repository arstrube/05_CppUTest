@echo off
set log=%~n0.LOG
echo echo|set /p=>%log%
for %%a in (1 2 EXT1 EXT2 EXT3 EXT4) do (
 echo Running CPPU%%a.EXE...
 echo *** CPPU%%a.EXE **************************************************>>%log%
 D:\bin\DOSBox\DOSBox.exe exit -c "CPPU%%a.EXE>>%log%" -noconsole -exit
)
echo Done.
