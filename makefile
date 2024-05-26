FLAGS= -ggdb -Wall -Wextra -std=gnu99

all: compile run

compile: src/main.c  
		gcc src/main.c ${FLAGS} -o ./build/main

run:
		./build/main
