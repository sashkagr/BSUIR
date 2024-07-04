call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
cl.exe /EHsc main.cpp
del main.obj
pause
cls
main.exe
cmd /K