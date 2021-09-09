@echo off

REM
REM To run this at startup, us this as your shortcut target:
REM %windir%\system32\cmd.exe /k w:\handmade\misc\shell.bat
REM
REM The subst w: drive is to abstract the absolute path so that
REM only the relitive path need be remembered and the code can
REM be ported to another machine. The "startup.bat" file facilitates
REM the setting of the subst directory as well as other user startup
REM batch programs to be run in the shell. It can be found in
REM	C:\Users\<username>\AppData\Microsoft\Windows\"Start Menu"\Programs\Startup
REM
REM If this file doesn't exist, it can be made or just remove "REM" from the below
REM and the W: drive will be set for you and then just type "W:" to jump to the directory.

REM W is for Working, get it? It's the Working directory. You should set
REM W to be where you intend to do your work. You can change it below
REM or in the "startup.bat" file above if it is there. If not you can
REM make it yourself.

call echo "running shell.bat..."
call subst w: c:\Users\tlc\Documents\Programming\tlcHandmade
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set path=w:\handmade\misc;%path%