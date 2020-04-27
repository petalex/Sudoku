build: dependencies compile clean
dependencies:
	@cp instructions.txt bin/instructions.txt
compile: build/main.o build/tui.o build/engine.o build/save-load.o
	@gcc -o bin/sudoku build/main.o build/tui.o build/engine.o build/save-load.o -lncurses
build/main.o: src/main.c
	@gcc -o build/main.o -c src/main.c
build/tui.o: src/tui.c
	@gcc -o build/tui.o -c src/tui.c
build/engine.o: src/engine.c
	@gcc -o build/engine.o -c src/engine.c
build/save-load.o: src/save-load.c
	@gcc -o build/save-load.o -c src/save-load.c
clean:
	@rm -f build/*
build-windows: dependencies-windows compile-windows clean-windows
dependencies-windows:
	@if not exist "tmp" @md -p "tmp"
	@where gcc > "tmp\gcc_path.txt"
	@set /p gcc_path=<"tmp\gcc_path.txt"
	@set mingw_root_path=%gcc_path:~0,-12%
	@if not exist "%mingw_root_path%\bin\pdcurses.dll" @copy "lib\pdcurses.dll" "%mingw_root_path%\bin\pdcurses.dll"
	@if not exist "%mingw_root_path%\lib\pdcurses.lib" @copy "lib\pdcurses.lib" "%mingw_root_path%\lib\pdcurses.lib"
	@if not exist "%mingw_root_path%\include\curses.h" @copy "include\curses.h" "%mingw_root_path%\include\curses.h"
	@if not exist "%mingw_root_path%\include\panel.h" @copy "include\panel.h" "%mingw_root_path%\include\panel.h"
	@if not exist "bin\instructions.txt" @copy "instructions.txt" "bin\instructions.txt"
	@del /q "tmp"
compile-windows: build\main.o build\tui.o build\engine.o build\save-load.o
	@if not exist "build" @md -p "build"
	@gcc -o "bin\sudoku.exe" "build\main.o" "build\tui.o" "build\engine.o" "build\save-load.o" -lpdcurses
build\main.o: src\main.c
	@gcc -o "build\main.o" -c "src\main.c"
build\tui.o: src\tui.c
	@gcc -o build\tui.o -c src\tui.c
build\engine.o: src\engine.c
	@gcc -o build\engine.o -c src\engine.c
build\save-load.o: src\save-load.c
	@gcc -o build\save-load.o -c src\save-load.c
clean-windows:
	@del /q "build"