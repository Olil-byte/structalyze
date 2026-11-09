all: bin/structalyze

bin/structalyze: src/main.c
	gcc -g -Wall $< -o $@

