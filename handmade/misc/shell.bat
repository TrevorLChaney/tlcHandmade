@echo off

REM
REM To run this at startup, us this as your shortuct target:
REM %windir%\system32\cmd.exe /k w:\handmade\misc\shell.bat
REM

call echo "running shell.bat..."
call subst w: c:\Users\tlc\Documents\HandmadeHero
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set path=w:\handmade\misc;%path%
w:
