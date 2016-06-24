CFLAGS += -I./include

all:
	gcc src/client.c -o client $(CFLAGS)
	gcc src/sha256.c -o sha256 $(CFLAGS)

.PHONY:
clean:
	rm -fr client sha256

