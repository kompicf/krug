flagslink  := -lm -lpthread -lSDL2
flagsbuild := -Wall -Wextra -Wpedantic -Iinclude -Ifont -g
objects := $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))
CC := cc

.PHONY: all release debug clean export-compile-flags

all: $(objects)
	$(CC) $(objects) -o exe $(flagslink) && ./exe

release: $(objects)
	$(CC) $(objects) -o exe -O3 $(flagslink) && ./exe

debug: $(objects)
	$(CC) $(objects) -o exe -g $(flagslink) && gdb ./exe

build/%.o:: src/%.c
	$(CC) -c $< -o $@ $(flagsbuild)

clean:
	rm build/*.o

export-compile-flags:
	echo "$(flagsbuild) $(flagslink)" | tr -s '[:space:]' '\n' > compile_flags.txt
