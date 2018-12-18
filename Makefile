UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS = -lGL -lGLEW -lglfw -ldl
	INCLUDES=-I ./includes
endif
ifeq ($(UNAME_S),Darwin)
FRAMEWORKS=-framework OpenGL

LIBS= -L/System/Library/Frameworks \
	  -lglfw
INCLUDES=-I./includes
SRC=third-party-source-code/glad.c
OBJ=bin/glad.o
endif

SRC += util/glutil.c \
	   src/CollisionDetector.cpp \
	   src/Geometry.cpp \
	   src/Material.cpp \
	   src/ModelLoader.cpp \
	   src/Node.cpp \
	   src/PlayerCamera.cpp \
	   src/TexturedCube.cpp \
	   src/TexturedRectangle.cpp \
	   src/Transformation.cpp \
	   src/main.cpp


OBJ += 	bin/glutil.o \
		bin/CollisionDetector.o \
		bin/Geometry.o \
		bin/Material.o \
		bin/ModelLoader.o \
		bin/Node.o \
		bin/PlayerCamera.o \
		bin/TexturedCube.o \
		bin/TexturedRectangle.o \
		bin/Transformation.o \
		bin/main.o

WARNINGS=-w

FLAGS=-std=c++11

all:
	g++ -c $(SRC) $(INCLUDES) $(WARNINGS) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	g++ $(OBJ) $(FRAMEWORKS) $(LIBS) $(INCLUDES) -o bin/main $(WARNINGS) $(FLAGS)
