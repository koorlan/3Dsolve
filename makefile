CC = gcc

SUFFIXES	?= .c .o
.SUFFIXES: $(SUFFIXES) .

CFLAGS = -g -Wall -D_REENTRANT `pkg-config --cflags glfw3 `
LDLIBS = `pkg-config --libs --static glfw3 glew ` `pkg-config --libs glu` -lpthread -lm


PROG  = snake

OBJS  = resolver.o listSolution.o snake.o log.o camera.o shader.o lodepng.o object.o context.o renderer.o main.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm *.o && rm snake
