# Description  : This is a makefile for the project, run this file to build the project

# default target
bench: bench.o getmem.o freemem.o mem_utils.o get_mem_stats.o print_heap.o
	gcc -Wall -g -std=c17 -o bench bench.o getmem.o freemem.o mem_utils.o get_mem_stats.o print_heap.o

# test cases
test1: bench
	./bench

test2: bench
	./bench 100000

test3: bench
	./bench 300000 50 50 4000 40000

test4: bench
	./bench 300000 50 50 400 40000

test5: bench
	./bench 1000000

# individual source files
getmem.o: getmem.c mem.h mem_impl.h
	gcc -Wall -g -std=c17 -c getmem.c

freemem.o: freemem.c mem_impl.h
	gcc -Wall -g -std=c17 -c freemem.c

get_mem_stats.o: get_mem_stats.c mem_impl.h
	gcc -Wall -g -std=c17 -c get_mem_stats.c

print_heap.o: print_heap.c mem_impl.h
	gcc -Wall -g -std=c17 -c print_heap.c

mem_utils.o: mem_utils.c mem_impl.h
	gcc -Wall -g -std=c17 -c mem_utils.c

bench.o: bench.c mem.h
	gcc -Wall -g -std=c17 -c bench.c

# remove built files and backups
clean:
	rm -f *.o *.txt test bench *~
