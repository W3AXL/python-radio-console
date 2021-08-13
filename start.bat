@echo off
set LOGFILE=last.log
call :LOG > %LOGFILE%
exit /B

:LOG
CALL .\Scripts\activate
CALL py server-runtime.py -c .\testCfg.json -sp 9995 -wp 4443