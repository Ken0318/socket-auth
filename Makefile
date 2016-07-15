CFLAGS += -I./include

all:
	gcc src/sha256.c src/client.c -o client $(CFLAGS)
	gcc src/sha256.c src/server-demo.c -o server-demo $(CFLAGS)

.PHONY:
clean:
	rm -fr client sha256

