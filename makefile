CC = gcc

SUFFIXES	?= .c .o
.SUFFIXES: $(SUFFIXES) .

CFLAGS = -g -Wall
LDLIBS = -lGLEW -lGLU -lGL -lglfw3 -lX11 -lXxf86vm -lXrandr -lXi -lXinerama -lXcursor -lm -lpthread
PROG  = snake

OBJS  = resolver.o listSolution.o snake.o log.o camera.o shader.o object.o context.o renderer.o main.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm *.o && rm snake
