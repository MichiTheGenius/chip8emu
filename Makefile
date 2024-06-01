# create final executable by linking all object files together   
emu.x: build/main.o build/chip8.o
	g++ build/main.o build/chip8.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o emu.x

# compile the source code files without linking -> create object files
build/main.o: src/main.cpp src/chip8.h
	@mkdir -p build
	g++ -c src/main.cpp -o build/main.o

build/chip8.o: src/chip8.cpp src/chip8.h
	@mkdir -p build
	g++ -c src/chip8.cpp -o build/chip8.o

clean:
	rm -rf build/*
