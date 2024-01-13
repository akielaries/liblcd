
all:
	gcc src/*.c samples/liblcd_time.c -o time -lc

clean:
	rm time
