@echo off
gcc src/*.c -Wall -I"C:\Users\ADMIN\c_projects\search_algorithms\include" -L"C:\Users\ADMIN\c_projects\search_algorithms\lib" -lmingw32 -lSDL2main -lSDL2 -o bin\main.exe 
pause