CC = gcc
CFLAGS = -Wall -Wextra -lm -g  -lGL -lpthread -lXrandr -lglfw -lx11 -lXi -ldl
LDFLAGS = -lglfw -lGL -lX11 -lm -lpthread -lXrandr -lXi -ldl
LDLIBS =

SRC = main.c shader.c glad.c math/matrix.c math/vec3.c math/vec4.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: main math/matrix

main: main.o glad.o shader.o math/matrix.o math/vec4.o

math/matrix: math/matrix.o math/vec4.o

.PHONY: clean
clean: 
	${RM} ${OBJ} ${EXE}
