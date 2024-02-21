flagslink  := -lm -lpthread -lSDL2
flagsbuild := -Wall -Wextra -Wpedantic -Iinclude
objects := $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))
TARGET = krug
CC := cc

.PHONY: all release debug clean export-compile-flags

all: $(objects)
	$(CC) $(objects) -o $(TARGET) $(flagslink) && ./$(TARGET)

release: $(patsubst %.o, %-O3.o, $(objects))
	$(CC) $(patsubst %.o, %-O3.o, $(objects)) -o $(TARGET) -O3 $(flagslink)

debug: $(patsubst %.o, %-g.o, $(objects))
	$(CC) $(patsubst %.o, %-g.o, $(objects)) -o $(TARGET) -g -fsanitize=address $(flagslink)

build/%.o:: src/%.c
	$(CC) -c $< -o $@ $(flagsbuild)

build/%-O3.o:: src/%.c
	$(CC) -c $< -o $@ $(flagsbuild) -O3

build/%-g.o:: src/%.c
	$(CC) -c $< -o $@ $(flagsbuild) -g -fsanitize=address

clean:
	rm -f build/*.o $(TARGET)

export-compile-flags:
	echo "$(flagsbuild) $(flagslink)" | tr -s '[:space:]' '\n' > compile_flags.txt
