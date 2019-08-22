.PHONY: clean
.PHONY: debug

picture : bmp.o canvas.o vector.o main.o
	gcc -o picture main.o bmp.o canvas.o vector.o -lm

canvas.o : canvas.c
	gcc -c canvas.c canvas.h

bmp.o : bmp.c
	gcc -c bmp.c bmp.h

vector.o : vector.c
	gcc -c vector.c vector.h

main.o : main.c
	gcc -c main.c

debug: 
	gcc -g main.c vector.c bmp.c canvas.c -o dbpic -lm

clean : 
	rm -r *.o


