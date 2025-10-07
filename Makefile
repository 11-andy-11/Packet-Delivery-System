CC = gcc
FLAGS = -Wall
PROGS = pachete
all: $(PROGS)

pachete: pachete.c
	$(CC) pachete.c -lm -o pachete $(FLAGS)

clean:
	rm -f *~ *.out $(PROGS)
