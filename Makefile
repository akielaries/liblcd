
all:
	gcc -Wall -pedantic -Werror -g src/*.c samples/liblcd_time.c -o time -lc

clean:
	rm time
