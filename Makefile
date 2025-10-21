CFLAGS = -g -O3


all: countbytes

countbytes: main.c
	$(CC) $(CFLAGS) main.c -o countbytes

clean:
	rm -f countbytes

.PHONY: clean
