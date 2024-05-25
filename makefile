FLAGS= -ggdb -Wall -Wextra

all: compile run

compile: src/main.c  
		cc src/main.c ${FLAGS} -o ./build/main

run:
		./build/main
