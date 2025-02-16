all: prog1 prog2

prog1: ./src/prog1.c
	gcc ./src/prog1.c -o prog1

prog2: ./src/prog2.c
	gcc ./src/prog2.c -o prog2

clean:
	rm -f prog1 prog2
