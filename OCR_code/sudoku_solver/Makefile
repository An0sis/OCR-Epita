# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g -lm
LDFLAGS =
LDLIBS =

OBJ = solverMain.o solver.o
DEP = ${OBJ:.o=.d}

all: solver

main: solverMain.c solver.c solver.h util.c util.h #Files used

clean:
	${RM} ${OBJ} ${DEP} solver
	${RM} *.result
	

solver:
	gcc -Wall -Wextra -std=c99 -O1 -o solver *.c *.h -g
-include ${DEP}

# END