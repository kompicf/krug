flagslink  := -lm -lpthread -lSDL2
flagsbuild := -Wall -Wextra -Wpedantic -Iinclude
objects := $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))
CC := cc

.PHONY: all release debug clean export-compile-flags

all: $(objects)
	$(CC) $(objects) -o exe $(flagslink) && ./exe

release: $(patsubst %.o, %-O3.o, $(objects))
	$(CC) $(patsubst %.o, %-O3.o, $(objects)) -o exe -O3 $(flagslink) && ./exe

debug: $(patsubst %.o, %-g.o, $(objects))
	$(CC) $(patsubst %.o, %-g.o, $(objects)) -o exe -g $(flagslink) && gdb ./exe

build/%.o:: src/%.c
	$(CC) -c $< -o $@ $(flagsbuild)

build/%-O3.o:: src/%.c
	$(CC) -c $< -o $@ $(flagsbuild) -O3

build/%-g.o:: src/%.c
	$(CC) -c $< -o $@ $(flagsbuild) -g

clean:
	rm build/*.o

export-compile-flags:
	echo "$(flagsbuild) $(flagslink)" | tr -s '[:space:]' '\n' > compile_flags.txt
