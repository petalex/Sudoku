build: compile depencencies clean
compile: build/main.o build/tui.o build/engine.o build/save-load.o
	@gcc -o bin/linux/sudoku build/main.o build/tui.o build/engine.o build/save-load.o -lncurses
build/main.o: src/main.c
	@gcc -o build/main.o -c src/main.c
build/tui.o: src/tui.c
	@gcc -o build/tui.o -c src/tui.c
build/engine.o: src/engine.c
	@gcc -o build/engine.o -c src/engine.c
build/save-load.o: src/save-load.c
	@gcc -o build/save-load.o -c src/save-load.c
depencencies:
	@cp instructions.txt bin/linux/instructions.txt
clean:
	@rm -f build/*