
all: monosolve

monosolve: monosolve.o dict.o score_words.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o monosolve monosolve.o dict.o score_words.o

clean:
	rm *.o monosolve

install: monosolve
	mkdir -p ../bin
	cp monosolve ../bin

.c.o:
	$(CC) $(CFLAGS) -c $<
