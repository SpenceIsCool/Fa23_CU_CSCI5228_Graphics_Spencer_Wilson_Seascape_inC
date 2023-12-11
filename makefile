# Homework 5
EXE=project

# Main target
all: $(EXE)

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
project.o: project.c SDW.h
loadtexbmp.o: loadtexbmp.c SDW.h
printers.o: printers.c SDW.h
spec.o: spec.c SDW.h
baseShapes.o: baseShapes.c SDW.h
shader.o: shader.c SDW.h
coord.o: coord.c SDW.h
alpha.o: alpha.c SDW.h
# z.o: z.c SDW.h
# NOTE: See below z.o as well, don't forget to extend SDW.a

#  Create archive
SDW.a:printers.o spec.o baseShapes.o loadtexbmp.o shader.o coord.o alpha.o  # z.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
project:project.o   SDW.a
	gcc $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
