CC = gcc

SUFFIXES	?= .c .o
.SUFFIXES: $(SUFFIXES) .

CFLAGS = -g -Wall `pkg-config --cflags glfw3 glew freetype2 `
LDLIBS = -L/usr/local/lib -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo `pkg-config --libs  glfw3 glew freetype2 ftgl` -lpthread -lm 
PROG  = snake

OBJS  = resolver.o listSolution.o snake.o log.o camera.o shader.o lodepng.o object.o context.o renderer.o fonts.o main.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS) 

.c.o:
	$(CC) $(CFLAGS) -c $*.c 

clean:
	rm *.o && rm snake

