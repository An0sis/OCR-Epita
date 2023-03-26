# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -std=c99 -lm -g -Iheader
LDFLAGS =
LDLIBS = 

SRC = ${wildcard ./src/*.c} 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

main_network: ${OBJ}
	${CC} $^ ${CFLAGS} -o main_network

main_network_debug: ${OBJ}
	${CC} $^ ${CFLAGS} -o main_network

	
all: main_network

debug: main_network_debug

.PHONY: clean

clean:
	rm -rf header/*.gch
	rm -rf src/*.o
	rm -rf main_network

# END