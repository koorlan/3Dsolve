CC = gcc

SUFFIXES	?= .c .o
.SUFFIXES: $(SUFFIXES) .

ifdef SystemRoot
 	RM = cmd /C del
 	EXT = .exe
	CFLAGS = -g -Wall -IC:/MinGW/include -IC:/MinGW/include/freetype2
	LDLIBS = -LC:/MinGW/lib -lfreetype -lftgl -mwindows -lglfw3 -lglew32 -lopengl32 -lpthread -lm
else
   ifeq ($(shell uname), Linux)
		RM = rm -f
		EXT = $1
		CFLAGS = -g -Wall `pkg-config --cflags glfw3 glew freetype2 `
		LDLIBS = -L/usr/local/lib -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo `pkg-config --libs  glfw3 glew freetype2 ftgl` -lpthread -lm
	 elif ($(shell uname), Darwin)
		RM = rm -f
		EXT = $1
		CFLAGS = -g -Wall `pkg-config --cflags glfw3 glew freetype2 `
		LDLIBS = -L/usr/local/lib -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo `pkg-config --libs  glfw3 glew freetype2 ftgl` -lpthread -lm
   endif
endif

PROG  = snake

OBJS  = resolver.o listSolution.o snake.o log.o camera.o shader.o lodepng.o object.o context.o renderer.o fonts.o main.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	$(RM) $(OBJS)
