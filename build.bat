@echo off
if not exist bin mkdir bin
gcc src/*.c -Wall -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -o bin\main.exe
copy /Y lib\SDL2.dll bin\ >nul
pause
