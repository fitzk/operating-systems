

default: tests

tests: dir_tests.h room_tests.h adventure.h 
	gcc -o tests dir_tests.c room_tests.c adventure.c testing_main.c 
	
clean:
	rm tests

cleanall: 
	rm tests
