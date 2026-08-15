CC = gcc

exec = xyz

sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)

CFLAGS = -Wall -Wextra -std=c11 -g

$(exec): $(objects)
	$(CC) $(objects) $(CFLAGS) -o $(exec)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(exec)
	./$(exec)

install: $(exec)
	sudo cp ./$(exec) /usr/local/bin/xyz

clean:
	rm -f $(objects) $(exec)

.PHONY: run install clean
