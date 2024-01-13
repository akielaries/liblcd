
all:
	gcc -std=c99 src/*.c samples/liblcd_time.c -o time -lc

clean:
	rm time
