CC = gcc
CFLAGS = -Wall -Wextra -lm -g  -lGL -lpthread -lXrandr -lglfw -lx11 -lXi -ldl
LDFLAGS = -lglfw -lGL -lX11 -lm -lpthread -lXrandr -lXi -ldl
LDLIBS =

MATH_SRC = math/matrix.c math/vec4.c math/vec3.c
3D_SRC = shader.c perspective.c camera.c
SRC = main.c glad.c ${MATH_SRC} ${3D_SRC}

OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: main 

main: ${OBJ}

math/matrix: math/matrix.o math/vec4.o

.PHONY: clean
clean: 
	${RM} ${OBJ} ${EXE}
