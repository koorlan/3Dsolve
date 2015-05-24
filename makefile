CC = gcc

SUFFIXES	?= .c .o
.SUFFIXES: $(SUFFIXES) .

OS := $(shell uname -s)

RM =  rm -f /cmd /C del
EXT = .exe
CFLAGS = -g -Wall -IC:/MinGW/include -IC:/MinGW/include/freetype2
LDLIBS = -LC:/MinGW/lib -mwindows -lglfw3 -lglew32 -lopengl32 -lfreetype -lftgl -lpthread -lm


ifdef SystemRoot
 	RM = cmd /C del
 	EXT = .exe
	CFLAGS = -g -Wall -IC:/MinGW/include -IC:/MinGW/include/freetype2
	LDLIBS = -LC:/MinGW/lib -mwindows -lglfw3 -lglew32 -lopengl32 -lfreetype -lftgl -lpthread -lm
else
   ifeq ($(OS),Linux)
		RM = rm -f
		EXT = $1
		CFLAGS = -g -Wall `pkg-config --cflags glfw3 glew freetype2 `
		LDLIBS = `pkg-config --libs --static glfw3 glew freetype2 ftgl` -lpthread -lm
	else
		ifeq ($(OS),Darwin)
			RM = rm -f
			EXT = $1
			CFLAGS = -g -Wall `pkg-config --cflags glfw3 glew freetype2 `
			LDLIBS = `pkg-config --libs --static  glfw3 glew freetype2 ftgl` -lpthread -lm
  		endif
   endif
endif

OBJS  = resolver.o listSolution.o snake.o log.o camera.o shader.o lodepng.o object.o context.o renderer.o fonts.o menu.o main.o

PROG = snake

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	$(RM) $(OBJS) && $(RM) $(PROG)$(EXT)
