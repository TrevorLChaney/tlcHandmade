.ONESHELL: # Use the same shell for all commands in a target.
CFLAGS=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4505 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -FC -Z7 -Fmwin32_handmade.map

LFLAGS=-opt:ref user32.lib gdi32.lib winmm.lib

all:
	build.bat

shell:
	@echo off
	call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

	IF NOT EXIST ..\..\build mkdir ..\..\build
	pushd ..\..\build

	REM 32-bit build
	REM cl %CommonCompilerFlags% ..\handmade\code\win32_handmade.cpp /link %CommonLinkerFlags% subsystem:windows,5.1

	REM 64-bit build
	REM Optimization Switches /O2 /Oi /fp:fast
	cl %CommonCompilerFlags% ..\handmade\code\handmade.cpp -Fmhandmade.map /LD /link -incremental:no -opt:ref /PDB:handmade_%random%.pdb /Export:GameGetSoundSamples /Export:GameUpdateAndRender
	cl %CommonCompilerFlags% ..\handmade\code\win32_handmade.cpp -Fmwin32_handmade.map /link %CommonLinkerFlags%

	popd
