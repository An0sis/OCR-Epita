# Makefile

CC = gcc
CPPFLAGS= `pkg-config --cflags sdl2`
CFLAGS = `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image` -O3 -g
LDFLAGS =
LDLIBS = -lm -lSDL2_image `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2 SDL2_image` -export-dynamic
BIN = ocr

all : ocr

ocr : ../image_pre-treatment/utils.o ../image_pre-treatment/point.o ../image_pre-treatment/rotate.o ../image_pre-treatment/static_black_and_white.o ../image_pre-treatment/thresholding.o ../image_pre-treatment/grayscale.o ../image_pre-treatment/median_blur.o ../image_pre-treatment/utils.o ../neural_network/src/initialize_network.o ../neural_network/src/result_network.o ../neural_network/src/learning_network.o ../neural_network/src/data_network.o ../neural_network/src/result_network.o ../sudoku_solver/util.o ../sudoku_solver/solver.o

clean :
	${RM} *.o
	${RM} *.d
	${RM} ./*.o
	${RM} ./*.d
	${RM} ../neural_network/src/*.o
	${RM} ../neural_network/src/*.d
	${RM} ../image_pre-treatment/*.o
	${RM} ../image_pre-treatment/*.d
	${RM} ../image_pre-treatment/image_pre-treatment
	${RM} ../sudoku_solver/*.d
	${RM} ../sudoku_solver/*.o
	${RM} *#*
	${RM} *~
	${RM} *.bmp
	${RM} *.result
	${RM} num/.txt
	${RM} $(BIN)

# END