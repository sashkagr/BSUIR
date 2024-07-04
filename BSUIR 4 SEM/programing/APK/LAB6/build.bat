@ECHO OFF
tasm /m2 lab6.asm
tlink /3 lab6.obj
del *.obj
del *.map