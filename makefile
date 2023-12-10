all: appendMeMore mydup part3

appendMeMore: appendMeMore.o
	gcc appendMeMore.o -o appendMeMore

appendMeMore.o: appendMeMore.c
	gcc -c -ansi -pedantic-errors -Wall *.c -std=gnu99 appendMeMore.c

mydup: mydup.o
	gcc mydup.o -o mydup

mydup.o: mydup.c
	gcc -c -ansi -pedantic-errors -Wall *.c -std=gnu99 mydup.c

part3: part3.o
	gcc part3.o -o part3

part3.o: part3.c
	gcc -c -ansi -pedantic-errors -Wall *.c -std=gnu99 part3.c

clean:
	rm -f appendMeMore mydup part3 *.o