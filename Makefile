main: main.o elevator.o person.o
	gcc -o main main.o elevator.o person.o -lncurses

main.o: main.c 
	gcc -c main.c

person.o: person.c person.h
	gcc -c person.c

elevator.o: elevator.c elevator.h
	gcc -c elevator.c

run: main
	./main

clean: 
	rm -f main *.o