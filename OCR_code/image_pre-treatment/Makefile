# Makefile

CC = gcc -g
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -I `pkg-config --cflags sdl2 SDL2_image` -std=c99
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -lSDL2_image

all: image_pre-treatment

SRC = ${wildcard *.c}
HEA = ${wildcard ./header/*h}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}
NNH = ${../neural_network/header/initialize_network.h ../neural_network/header/result_network.h ../neural_network/header/learning_network.h ../neural_network/header/data_network.h ../neural_network/header/result_network.h}
NN = ${../neural_network/src/initialize_network.o ../neural_network/src/result_network.o ../neural_network/src/learning_network.o ../neural_network/src/data_network.o ../neural_network/src/result_network.o}

image_pre-treatment: ${OBJ} ${NN}

%.o: %.c ${HEA} ${NNH} 

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf image_pre-treatment

# END
