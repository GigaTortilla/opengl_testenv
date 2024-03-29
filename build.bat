@echo off
rem This part compiles the camera functions into a dynamic windows library incl archive file
gcc -c -o camdll.o .\src\camera\cam.c -D CAMDLL_EXPORTS -I .\include
gcc -shared -o libcamdll.dll .\camdll.o -L .\lib -lglfw3dll
del camdll.o
gendef libcamdll.dll
dlltool --def libcamdll.def --dllname libcamdll.dll --output-lib lib\libcamdll.a
del libcamdll.def
echo on

gcc -o test_file .\src\*.c -I .\include -L .\lib -lglfw3dll -lcglm.dll -lcamdll -mwindows -Wall -Wextra
pause