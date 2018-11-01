all: dir_info.o
	gcc -o out dir_info.o

dir_info.o: dir_info.c
	gcc -c dir_info.c

run:
	./out

clean:
	rm out
	rm *.o
