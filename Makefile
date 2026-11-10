all: bin/structalyze

obj/matrix.o: src/matrix.c src/matrix.h
	gcc -c -g -Isrc/ -Wall $< -o $@

obj/cmd.o: src/cmd.c src/cmd.h
	gcc -c -g -Isrc/ -Wall $< -o $@

bin/structalyze: src/main.c obj/cmd.o obj/matrix.o
	gcc -g -Isrc/ -Wall $< obj/cmd.o obj/matrix.o -o $@ -lm

